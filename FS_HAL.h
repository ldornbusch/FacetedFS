// FS_HAL.h: Schnittstelle für die Klasse FS_HAL.
// 
// FS_HAL is a small abstraction layer wich will provide 
// the funtionality to create hard_links and store data in 
// ADSs. All methods which operate to the outside will communicate 
// by this class. The class contains static functions, so it is possible 
// to use the methods without an instance.
// However you have to call FS_HAL::unload() to be sure, that all
// resources will be freed. usually this will happen on end of your program.
// 
// This class will load the Kernel32.dll and store it into a static var
// also it will retrieve the pointer to the CreateHardLink-function
// The DLL will not be freed unless FS_HAL::unload() is called.
// on each creation of a hard link it will be checked, if the dll was loaded
// if not it will be loaded
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FS_HAL_H__7386CB2A_76B3_4459_9F73_C653F03A0E2E__INCLUDED_)
#define AFX_FS_HAL_H__7386CB2A_76B3_4459_9F73_C653F03A0E2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

#include "app.h"

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
		static DWORD createHardLink(std::string DestPath, std::string SourcePath);
		static DWORD init();
		static void unload();
	};
}
#endif // !defined(AFX_FS_HAL_H__7386CB2A_76B3_4459_9F73_C653F03A0E2E__INCLUDED_)
