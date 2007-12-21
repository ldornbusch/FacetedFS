// File.h: Schnittstelle für die Klasse File.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILE_H__D32C6255_C161_4220_A63C_3D5219871EA7__INCLUDED_)
#define AFX_FILE_H__D32C6255_C161_4220_A63C_3D5219871EA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <map>

#include "tag.h"
#include "app.h"

namespace APP_NAME{

	class File  
	{
	public:
		File();
		File(std::string name);
		virtual ~File();
		std::string file_name;
		std::map<std::string, APP_NAME::Tag*> map_FileTags;

		void setTag(Tag&);
		bool hasTag(const std::string strTagName) const ;
		Tag& getTag(const std::string strTagName) ;
	};
}
#endif // !defined(AFX_FILE_H__D32C6255_C161_4220_A63C_3D5219871EA7__INCLUDED_)
