// TagFactory.cpp: Implementierung der Klasse TagFactory.
//
//////////////////////////////////////////////////////////////////////

#include "TagFactory.h"
#include "StringTag.h"
#include "NumberTag.h"


Tag* TagFactory::createTag(std::string TagName, tag_type tagType, void* data){
	Tag* retVal = 0;
	switch(tagType){
		case  TAG_TYPE_BOOL:
			retVal = new Tag(TagName);
			break;
		case  TAG_TYPE_NUMBER:
			retVal = new NumberTag(TagName,*((double*)data));
			break;
		case  TAG_TYPE_STRING:
			std::string TagValue = (char*) data;
      retVal = new StringTag(TagName, TagValue);
			break;
	}
	return retVal;
}