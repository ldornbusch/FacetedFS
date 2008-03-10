// FS_HAL.cpp: Implementierung der Klasse FS_HAL.
//
//////////////////////////////////////////////////////////////////////


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
 
