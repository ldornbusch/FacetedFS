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

	enum tag_type{	
		TAG_TYPE_BOOL, 
		TAG_TYPE_NUMBER, 
		TAG_TYPE_STRING 
	};

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
		virtual int getLength() const { return 0;	}
		virtual tag_type getType() const { return TAG_TYPE_BOOL;}
		virtual void writeValue(void* memptr) const { }
	};
}
#endif // !defined(AFX_TAG_H__71B189AA_5594_481B_803B_339DF24D0F71__INCLUDED_)
