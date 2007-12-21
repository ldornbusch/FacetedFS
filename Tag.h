// Tag.h: Schnittstelle für die Klasse Tag.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAG_H__71B189AA_5594_481B_803B_339DF24D0F71__INCLUDED_)
#define AFX_TAG_H__71B189AA_5594_481B_803B_339DF24D0F71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

#include "app.h"

namespace APP_NAME{

	class Tag  
	{
		std::string tag_name;		
	public:
		Tag();
		Tag(std::string tag_name);
		virtual ~Tag();
		// this function will alway return a string representation of  which can 
		std::string getTagname() const{ return this->tag_name;};
		virtual std::string getStringValue() const { return "true";	}
	};
}
#endif // !defined(AFX_TAG_H__71B189AA_5594_481B_803B_339DF24D0F71__INCLUDED_)
