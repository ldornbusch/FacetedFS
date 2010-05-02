// FS_HAL.cpp: Implementierung der Klasse FS_HAL.
//
//////////////////////////////////////////////////////////////////////
#pragma warning( disable :  4786 )


#include <stdio.h>
#include <tchar.h>

#pragma comment( lib, "advapi32.lib" )

#include "FS_HAL.h"
#include "TagFactory.h"

using namespace APP_NAME;
// static Membervariables: pointer to dll-function and 
// Module handle for library
DLLCreateHardLink FS_HAL::pfCreateHardLink=0;
HMODULE FS_HAL::Kernel32=0;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

FS_HAL::FS_HAL()
{
	init();
}

FS_HAL::~FS_HAL()  
{
	unload();
}

/** 
	Function to load the kernel32.dll and retrieve the pointer for the 

	CreateHardLink-Function

	You can call this function multiple times, it will check if the dll 
	is already loaded
*/
DWORD FS_HAL::init()
{
	DWORD retVal=0;
	if (pfCreateHardLink==0 || Kernel32==0){
		 Kernel32 = LoadLibrary( _T( "kernel32.dll" ) ) ;
		 if ( Kernel32 > (HMODULE)32 ){	
			FS_HAL::pfCreateHardLink = 
				(DLLCreateHardLink) GetProcAddress( Kernel32, "CreateHardLinkA" );
			if (FS_HAL::pfCreateHardLink == 0){	
				retVal = GetLastError();
				FreeLibrary( Kernel32 );
				Kernel32 = 0;
				pfCreateHardLink = 0;
			}
		}else{
			retVal = GetLastError();
			FreeLibrary( Kernel32 );
			Kernel32 = 0;
			pfCreateHardLink = 0;
		}
	}
	return retVal;
}

/**
	Function to unload Kernel32.dll you can call this function also 
	multiple times, as it checks the state of the ModuleHandler 
	on each call.
*/
void FS_HAL::unload()
{
		if (Kernel32 != 0){
			FreeLibrary( Kernel32 ) ;
		}
		Kernel32 = 0;
		pfCreateHardLink = 0;
}

/**
	This Function will create a HardLink from a file. 
	@param	std::string DestPath		The TargetFile where the link will be 
	                                created
	@param	std::string SourcePath	The SourceFile where the link should 
	                                be pointing to
	@returns	returns the Windows ErrorCode, if operation failed, 
						if operation succeeded, function will return 0
*/
DWORD FS_HAL::createHardLink(std::string DestPath, std::string SourcePath)
{
	DWORD retVal=0;
	init();
	if ( !pfCreateHardLink( DestPath.c_str(), SourcePath.c_str(), NULL )){
	  // API call failed
    retVal = GetLastError() ;
	}
	return retVal;
}
 
/** load(File& ) Function will load all tags and will load them into the 
	file object which was given as parameter. The filename will be retrieved 
	by parameter
	@param File			File class which represents the file which tags 
	                should be loaded
	@param filter		(optional) if this vector is nonempty, only tags with 
	                matching names will be loaded
	@returns        0, if no error occured or errorcode
*/
int FS_HAL::load(File& file, const std::vector<std::string> filter){
	int retVal=FILE_NOT_FOUND;
	ChunkContainer	TagContainer;
	TagFile		FileHeader;
	file.map_FileTags.clear();
	std::string adsFileName = file.file_name+ADS_NAME ;
	FILE* Handle = fopen(adsFileName.c_str(), "r");
	if (Handle != 0){
		retVal=FILE_UNSUPPORTED_FORMAT;
		fread(&FileHeader.FormatSpecifier,4,1,Handle);
		if (feof(Handle)){
			retVal=FILE_EMPTY;
		}
		fread(&FileHeader.MajorVersion,1,1,Handle);
		fread(&FileHeader.MinorVersion,1,1,Handle);
		if (  FileHeader.FormatSpecifier == FILE_ID
		   && FileHeader.MajorVersion == 0
			 && FileHeader.MinorVersion == 1){

			retVal=FILE_OK;
			fread(&TagContainer.ChunkID,1,1,Handle);
			fread(&TagContainer.ChunkLen,4,1,Handle);
			std::vector<TagChunk*> TagNames;

      int bytesread=sizeof_ChunkContainer;
			while(bytesread< TagContainer.ChunkLen){
				TagChunk* tmpTag = new TagChunk();
				fread(&tmpTag->ChunkLen,4,1,Handle);
				fread(&tmpTag->ChunkOffset,4,1,Handle);
				fread(&tmpTag->TagType,1,1,Handle);
				retVal = FILE_OUT_OF_MEMORY;
				// one additional byte for the \0:
				BYTE* mem = (BYTE*)malloc(sizeof(BYTE)*tmpTag->ChunkLen-9 + 1);	
        if (mem != 0){
					fread(mem, tmpTag->ChunkLen-9,1, Handle);
					mem[tmpTag->ChunkLen-9] = 0;
					tmpTag->TagName = (char*) mem;
					free(mem);
					mem=0;
					bytesread += tmpTag->ChunkLen;
          // check if this tag was requested..
					bool requested = false;
					std::vector<std::string>::const_iterator it;
					for (it=filter.begin(); it!=filter.end(); ++it){
						if (tmpTag->TagName.compare(*it)==0){
							requested=true;
							break;
						}
					}
					if (requested || filter.size()==0){
						TagNames.push_back(tmpTag);	
					} else {
						delete tmpTag;
					}
        }
			}
			/* 
				now we have a vector full of tagstructures and we now 
				going to load the appropiate Values and after this we 
				will construct the tag-Objects and add them to file
			*/
			std::vector<TagChunk*>::const_iterator it;
			for (it=TagNames.begin(); it != TagNames.end(); ++it){
        if ((*it)->TagType != TAG_TYPE_BOOL){	
					// BOOL TAGS do not have any value chunks
					ValueChunk tmpVal;
					fseek(Handle,(*it)->ChunkOffset ,SEEK_SET);
					fread(&tmpVal.ChunkLen,4,1,Handle);
					// here also we need an extra byte for delimiting our string
					BYTE* mem = (BYTE*)malloc(sizeof(BYTE)*tmpVal.ChunkLen-4 + 1); 
					if(mem != 0){
						fread(mem, tmpVal.ChunkLen-4,1, Handle);
						mem[tmpVal.ChunkLen-4] = 0;
						Tag* myTag = TagFactory::createTag((*it)->TagName, 
							           static_cast<tag_type>((*it)->TagType),
												 mem );
						file.setTag(*myTag);
						free(mem);
						mem = 0;
					}
				} else {	
					/**  creation of BOOL Tags is very simple because no extra 
					     data is read from file
					*/
					Tag* myTag = TagFactory::createTag((*it)->TagName, TAG_TYPE_BOOL, 0);
					file.setTag(*myTag);
				}	
			}
			for (it=TagNames.begin(); it != TagNames.end(); ++it){
				delete (*it);
			}
			retVal = FILE_OK;
		}
	}
	return retVal;	
}

/** save(const File& ) Function will save all tags and will save them 
	into the file object which was given as parameter. 
	The filename will be retrieved by parameter.
	***BE CAREFUL!! ***
	You must load all Tags(so load(File) without filter parameter!), 
	before you save the tags, otherwise all tags which were not loaded 
	will be lost, as we write the complete Tags file from scratch!

	How we work: 
	please read documentation for fileformat-specification

	@param File file which tags should be saved
	@returns int: 0, if no error occured or errorcode
*/
int FS_HAL::save(const File& file){

	std::map<std::string, APP_NAME::Tag*>::const_iterator it = 
		file.map_FileTags.begin();

	// 1st calculate some Values for determining the offsets into written file
	int TagLen = 0, ValLen = 0, TagCount = 0, ValCount = 0;
	for (it=file.map_FileTags.begin();it!=file.map_FileTags.end(); ++it){
		TagLen += it->first.length();
		++TagCount;
		if (it->second->getLength()!=0){
			ValLen += it->second->getLength();
			++ValCount;
		}
	}

  int TagsChunkSize = sizeof_ChunkContainer 
												+ TagCount * sizeof_TagChunk 
												+ TagLen ;

  int ValuesChunkSize = sizeof_ChunkContainer
													+ ValCount * sizeof_ValueChunk
													+ ValLen ;

	int FileSize =	sizeof_TagFile 
										+ TagsChunkSize
										+ ValuesChunkSize ;
	std::string adsFileName = file.file_name+ADS_NAME ;
	FILE* Handle = fopen(adsFileName.c_str(), "w");

	TagFile FileHeader;
	ChunkContainer Tags, Values;
	FileHeader.FormatSpecifier = FILE_ID;
	FileHeader.MajorVersion = 0;
	FileHeader.MinorVersion = 1;
	Tags.ChunkID = CHUNK_ID_TAGS;
	Tags.ChunkLen = TagsChunkSize;
	Values.ChunkID = CHUNK_ID_VALS;
	Values.ChunkLen = ValuesChunkSize;

	// 1st: write Fileheader 
	fwrite(&FileHeader.FormatSpecifier ,4, 1, Handle);
	fputc(FileHeader.MajorVersion, Handle);
	fputc(FileHeader.MinorVersion, Handle);

	// 2nd: write TagContainer Header
	fputc(Tags.ChunkID, Handle);
	fwrite(&Tags.ChunkLen, 4, 1, Handle);

	// 3rd: write each TagChunk
	// this variable sums up all bytes in the Valuechunk 
	// of variable length and helps us to point to the offset 
	// where the Valuechunk will be written
	int ValueOffset = 0;		
	// This variable sums up all Tags which have ValueChunk
	int ValueCount = 0;			
	for (it=file.map_FileTags.begin();it!=file.map_FileTags.end(); ++it){
		TagChunk Tag2Write;
		Tag2Write.ChunkLen = sizeof_TagChunk + it->first.length();
		Tag2Write.TagType = it->second->getType();
		Tag2Write.ChunkOffset = 0; 
		if (Tag2Write.TagType != 0){
			Tag2Write.ChunkOffset = sizeof_TagFile 
				                    + Tags.ChunkLen 
				                    + sizeof_ChunkContainer 
			                      + (sizeof_ValueChunk * ValueCount++) 
														+ ValueOffset;
		}
		ValueOffset+= it->second->getLength();

		fwrite(&Tag2Write.ChunkLen, 4, 1, Handle);
		fwrite(&Tag2Write.ChunkOffset, 4, 1, Handle);
		fwrite(&Tag2Write.TagType, 1, 1, Handle);
		fwrite(it->first.c_str(), it->first.length(), 1, Handle);
	}

	// 4th: write ValuesContainer Header
	fputc(Values.ChunkID, Handle);
	fwrite(&Values.ChunkLen, 4, 1, Handle);


// 5th:write each ValueChunk
	for (it=file.map_FileTags.begin();it!=file.map_FileTags.end(); ++it){
		ValueChunk Val2Write;
    if (it->second->getType()!=0){
			Val2Write.ChunkLen = sizeof_ValueChunk + it->second->getLength();
	    BYTE* mem = (BYTE*) malloc(sizeof(BYTE)*it->second->getLength());
			it->second->writeValue(mem);
			fwrite(&Val2Write.ChunkLen, 4, 1, Handle);
			fwrite(mem, it->second->getLength(), 1, Handle);
			free(mem);
			mem = 0;
    }
	}
	fclose(Handle);
	return 0;
}