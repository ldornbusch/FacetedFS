// StringTag.cpp: Implementierung der Klasse StringTag.
//
//////////////////////////////////////////////////////////////////////
#pragma warning( disable :  4786 )
#include "StringTag.h"
#include <sstream>
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
using namespace APP_NAME;

StringTag::StringTag()
{
	value="";
}
StringTag::StringTag(std::string name, std::string val) : Tag(name), value(val)
{
}

StringTag::~StringTag()
{

}

std:: string StringTag::getStringValue() const {
	return this->value;
}

std::string StringTag::getValue() const {
	return this->value;
}

void StringTag::setValue(std::string val){
	this->value = val;
}