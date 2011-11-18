// FS_HAL.h: Schnittstelle für die Klasse FS_HAL.
// 
// FS_HAL is a small abstraction layer wich will provide 
// the funtionality to create directories, hard_links and load/store data in 
// ADSs. All methods which operate to the outside (the OS[Windows]) will communicate 
// through this class. The class contains static functions, so it is possible 
// to use the methods without an instance.
// However you have to call FS_HAL::unload() to be sure, that all
// resources will be freed. usually this will happen at the end of your program.
// 
// The Idea was to make it easier to port this application to a different platform
// (dont know if it is possible!), or change the storage format/procedure.
//
// This class will load the Kernel32.dll and store it into a static var
// also it will retrieve the pointer to the CreateHardLink-function
// The DLL will not be freed unless FS_HAL::unload() is called.
// on each creation of a hard link it will be checked, if the dll was loaded
// if not it will be loaded
//
// For loading and saving Tags it will use an AlternateDataStream into which 
// the tags will be written by an binary format.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FS_HAL_H__7386CB2A_76B3_4459_9F73_C653F03A0E2E__INCLUDED_)
#define AFX_FS_HAL_H__7386CB2A_76B3_4459_9F73_C653F03A0E2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

#include "app.h"
#include "file.h"
#include <windows.h>
extern "C"
{
 typedef BOOL (__stdcall *DLLCreateHardLink)( LPCTSTR DestFile, LPCTSTR SourceFile, LPSECURITY_ATTRIBUTES SecAttr );
}



namespace APP_NAME{

	class FS_HAL  
	{
	private:
		static DLLCreateHardLink pfCreateHardLink;
		static HMODULE Kernel32;
	public:
		FS_HAL();
		virtual ~FS_HAL();
		static DWORD createHardLink(const std::string DestPath, const std::string SourcePath);
		static DWORD createDirectory(const std::string DestPath);
		static DWORD init();
		static void unload();
		static int load(File& file, const std::vector<std::string> filter=std::vector<std::string>());
		static int save(const File& file);
	};
	enum file_error{
		FILE_OK,
		FILE_NOT_FOUND,
		FILE_EMPTY,
		FILE_UNSUPPORTED_FORMAT,
		FILE_OUT_OF_MEMORY
	};

	enum chunk_ids{ 
		CHUNK_ID_DUMMY,					// matched to 0
		CHUNK_ID_TAGS,
		CHUNK_ID_VALS
	};	
	#define FILE_ID 'GTTM'	// THE ID means: MeTaTaG (without vowels)
	const int sizeof_TagFile  = 6 ;  // we cannot use sizeof, as it may contain paddingbytes!
	const int sizeof_TagChunk = 9 ;
	const int sizeof_ValueChunk = 4 ;
	const int sizeof_ChunkContainer = 5 ;


	struct ChunkContainer{
		BYTE	ChunkID;						// 0x01(CHUNK_ID_TAGS) oder 0x02 (CHUNK_ID_VALS)
		DWORD ChunkLen;						// len of chunk in Bytes
		void** Chunkdata;					// ChunkLen Bytes of Memorydata
	};

	struct TagFile{
		DWORD		FormatSpecifier;		// "MTTG"
		BYTE		MajorVersion;				// 0x0001 ()
		BYTE		MinorVersion;				// 0x0001 ()
		ChunkContainer**	chunks;							// list of chunks
	};

	struct ValueChunk{
			DWORD ChunkLen;						// len of chunk in Bytes
	};

	struct TagChunk : ValueChunk{
			DWORD ChunkOffset;				// offset to value chunk (relative to begin of file in Bytes)
      BYTE	TagType;            // 0x00=bool, 0x01=number, 0x02=string
			std::string TagName;					// arbitrary length data
	};

}
#endif // !defined(AFX_FS_HAL_H__7386CB2A_76B3_4459_9F73_C653F03A0E2E__INCLUDED_)
