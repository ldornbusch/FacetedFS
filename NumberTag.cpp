// NumberTag.cpp: Implementierung der Klasse NumberTag.
//
//////////////////////////////////////////////////////////////////////
#pragma warning( disable :  4786 )

#include "NumberTag.h"
#include <sstream>
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
using namespace APP_NAME;

NumberTag::NumberTag()
{
	value = 0.0;
}

NumberTag::NumberTag(std::string name, double val) : Tag(name), value(val)
{
}

NumberTag::~NumberTag()
{

}

std:: string NumberTag::getStringValue() const {
	std::string retVal;
	std::stringstream stream;
	stream << this->value;
	stream >> retVal;
	return retVal;
}

double NumberTag::getValue() const {
	return this->value;
}

void NumberTag::setValue(double val){
	this->value = val;
}