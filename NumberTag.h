// NumberTag.h: Schnittstelle f�r die Klasse NumberTag.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NUMBERTAG_H__CCEDB2B9_1C10_4A12_A25B_BB9EFF5A19FC__INCLUDED_)
#define AFX_NUMBERTAG_H__CCEDB2B9_1C10_4A12_A25B_BB9EFF5A19FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app.h"

#include "Tag.h"

namespace APP_NAME{

	class NumberTag : public Tag  
	{
	public:
		NumberTag();
		NumberTag(std::string name, double val);
		virtual ~NumberTag();
		std::string getStringValue() const ;
		double getValue() const ;
		void setValue(double val);
		virtual int getLength() const { return sizeof(double); };
		virtual tag_type getType() const { return TAG_TYPE_NUMBER;}
		virtual void writeValue(void* memptr) const { *(double*)memptr = value;}
	private:
		double value;
	};
}

#endif // !defined(AFX_NUMBERTAG_H__CCEDB2B9_1C10_4A12_A25B_BB9EFF5A19FC__INCLUDED_)
