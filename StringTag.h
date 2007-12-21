// StringTag.h: Schnittstelle für die Klasse StringTag.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGTAG_H__DFBCA5A5_EC4A_4983_B599_AF5B12FD8E9F__INCLUDED_)
#define AFX_STRINGTAG_H__DFBCA5A5_EC4A_4983_B599_AF5B12FD8E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app.h"

#include "Tag.h"

namespace APP_NAME{

	class StringTag : public Tag  
	{
	public:
		StringTag();
		StringTag(std::string name, std::string val);
		virtual ~StringTag();
		std::string getStringValue() const ;
		std::string getValue() const ;
		void setValue(std::string val);

	private:
		std::string value;
	};
}

#endif // !defined(AFX_STRINGTAG_H__DFBCA5A5_EC4A_4983_B599_AF5B12FD8E9F__INCLUDED_)
