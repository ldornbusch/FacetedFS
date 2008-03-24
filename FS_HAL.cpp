// FS_HAL.cpp: Implementierung der Klasse FS_HAL.
//
//////////////////////////////////////////////////////////////////////
#pragma warning( disable :  4786 )


#include <stdio.h>
#include <tchar.h>

#pragma comment( lib, "advapi32.lib" )

#include "FS_HAL.h"

using namespace APP_NAME;
// static Membervariables: pointer to dll-function and Module handle for library
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
	Function to load the kernel32.dll and retrieve the pointer for the CreateHardLink-Function
	You can call this function multiple times, it will check if the dll is already loaded
*/
DWORD FS_HAL::init()
{
	DWORD retVal=0;
	if (pfCreateHardLink==0 || Kernel32==0){
		 Kernel32 = LoadLibrary( _T( "kernel32.dll" ) ) ;
		 if ( Kernel32 > (HMODULE)32 ){	
			FS_HAL::pfCreateHardLink = (DLLCreateHardLink) GetProcAddress( Kernel32, "CreateHardLinkA" ) ;
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
	Function to unload Kernel32.dll you can call this function also multiple times, as it 
	checked the state of the ModuleHandler on each call.
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
	@param	std::string DestPath		The TargetFile where the link will be created
	@param	std::string SourcePath	The SourceFile where the link should be pointing to
	@returns	returns the Windows ErrorCode, if operation failed, 
						if operation succeeded, Function will return 0
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
@param File			File class which represents the file which tags should be loaded
@param filter		(optional) if this vector is nonempty, only tags with matching names will be loaded
*/
void FS_HAL::load(File& file, const std::vector<std::string> filter){
	
}

/** save(const File& ) Function will save all tags and will save them into the 
file object which was given as parameter. The filename will be retrieved 
by parameter.
***BE CAREFUL!! ***
You must load all Tags(so load(File) without filter parameter!), before you save 
the tags, otherwise all tags which were not loaded will be lost, as we write 
the complete Tags file new
How we work: 
please read documentation for fileformat-spezification
@param File file which tags should be saved
*/
void FS_HAL::save(const File& file){

	std::map<std::string, APP_NAME::Tag*>::const_iterator it = file.map_FileTags.begin();

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

	const int sizeof_TagFile  = 6 ;  // we cannot use sizeof, as it may contain paddingbytes!
	const int sizeof_TagChunk = 9 ;
	const int sizeof_ValueChunk = 4 ;
	const int sizeof_ChunkContainer = 5 ;

  int TagsChunkSize = sizeof_ChunkContainer 
												+ TagCount * sizeof_TagChunk 
												+ TagLen ;

  int ValuesChunkSize = sizeof_ChunkContainer
													+ ValCount * sizeof_ValueChunk
													+ ValLen ;

	int FileSize =	sizeof_TagFile 
										+ TagsChunkSize
										+ ValuesChunkSize ;

  FILE* Handle = fopen(file.file_name.c_str(), "a");

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
	int ValueOffset = 0;		// this variable sums up all bytes in the Valuechunk of variable length and helps us to point to the offset where the Valuechunk will be written
	int ValueCount = 0;			// This Variable sums up all Tags which have ValueChunk
	for (it=file.map_FileTags.begin();it!=file.map_FileTags.end(); ++it){
		TagChunk Tag2Write;
		Tag2Write.ChunkLen = sizeof_TagChunk + it->first.length();
		Tag2Write.TagType = it->second->getType();
		Tag2Write.ChunkOffset = 0; 
		if (Tag2Write.TagType != 0){
			Tag2Write.ChunkOffset = sizeof_TagFile 
															+ Tags.ChunkLen 
															+ sizeof_ChunkContainer 
															+ (sizeof_ValueChunk * ValueCount++) +ValueOffset;
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
    }
	}
	fclose(Handle);
}