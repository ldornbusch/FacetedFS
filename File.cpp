// File.cpp: Implementierung der Klasse File.
//
//////////////////////////////////////////////////////////////////////
#pragma warning( disable :  4786 )
#include "file.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
using namespace APP_NAME;

File::File()
{

}

File::File(std::string name)
{
	this->file_name = name;
}

File::~File()
{

}

void File::setTag(Tag& tag2set){
	if (tag2set.getTagname().length()>0){
		this->map_FileTags[tag2set.getTagname()]=&tag2set;
	}
}

void File::removeTag(std::string TagName){
	std::map<std::string,APP_NAME::Tag*>::iterator it;
	it = this->map_FileTags.find(TagName);
	if (it != this->map_FileTags.end()){
		this->map_FileTags.erase(it);
	}
}

bool File::hasTag(const std::string strTagName) const {
	return this->map_FileTags.find(strTagName)!=map_FileTags.end();
}

Tag& File::getTag(const std::string strTagName) {
	Tag* retVal;
	if (this->hasTag(strTagName)){
		retVal = this->map_FileTags[strTagName];
	}
	return *retVal;
}