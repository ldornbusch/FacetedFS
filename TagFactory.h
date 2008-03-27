// TagFactory.h: Schnittstelle für die Klasse TagFactory.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAGFACTORY_H__2CE9A70E_85D0_405E_A994_52C7C9DB6AC9__INCLUDED_)
#define AFX_TAGFACTORY_H__2CE9A70E_85D0_405E_A994_52C7C9DB6AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tag.h"
	using namespace APP_NAME;

class TagFactory  
{
public:
	static Tag* createTag(std::string Tagname, tag_type tagType, void* data);	// this function will create a tag of a specific type out of pure data
};

#endif // !defined(AFX_TAGFACTORY_H__2CE9A70E_85D0_405E_A994_52C7C9DB6AC9__INCLUDED_)
