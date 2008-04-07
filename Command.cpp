// command.cpp: Implementierung der Klasse command.
//
//////////////////////////////////////////////////////////////////////
#pragma warning( disable :  4786 )

#include <stdio.h>
#include <sstream>

#include "Command.h"

#include "FS_HAL.h"
#include "bucket.h"
#include "NumberTag.h"
#include "StringTag.h"

using namespace APP_NAME;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
using namespace APP_NAME;
int Command::get(std::string Filename, std::vector<std::string> TagList){
  int retVal=0;
	File myFile(Filename);
	retVal = FS_HAL::load(myFile, TagList);
	if (retVal == 0){
		if (TagList.size() == 0 ||  myFile.map_FileTags.size() == TagList.size()){
			std::map<std::string, APP_NAME::Tag*>::const_iterator it;

			std::stringstream outputstream;
			std::string outputstring;
			outputstream << myFile.file_name << ":" ;
			outputstream >> outputstring;
			printf(outputstring.c_str());
			printf("\n");
			for (it = myFile.map_FileTags.begin(); it != myFile.map_FileTags.end();++it){
        outputstream.clear();
				outputstream << it->first << "[" << Command::getTypeName(it->second->getType()) << "]:" ;
				outputstream >> outputstring;
				printf(outputstring.c_str());
				outputstream.clear();
				if (it->second->getType()>0){
					printf("\n");
					outputstream << it->second->getStringValue();
					outputstream >> outputstring;
					printf(outputstring.c_str());
				}
				printf("\n     --- separator ---\n");
			}
		}	else{
			retVal=5; // TODO:UNIFY ERROR CODES   This one means: TAGS NOT MATCHING
		}
	}
	return retVal;
}

int Command::set(std::string FileName, std::string TagName, std::string TagValue){
	int retVal = 0;
	if (TagValue.length() == 0){
		retVal = Command::set(FileName, TagName, TagValue, "BOOL");
	} else{
		retVal = Command::set(FileName, TagName, TagValue, "TEXT");
	}
	return retVal;
}

int Command::set(std::string FileName, std::string TagName, std::string TagValue, std::string TagType){
	int retVal = 0;
	File myFile(FileName);
	retVal = FS_HAL::load(myFile, std::vector<std::string>());
	if (retVal == 0 || retVal == FILE_EMPTY){
    Tag * myTag = 0;
		retVal = 6 ;  // TODO:UNIFY ERROR CODES   This one means: TAGTYPE NOT IMPLEMENTED
		if (TagType == "BOOL"){
			myTag = new Tag(TagName);
		}
		if (TagType == "NUMBER"){
			double value = atof(TagValue.c_str());
			myTag = new NumberTag(TagName, value);
		}
		if (TagType == "TEXT"){
			myTag = new StringTag(TagName, TagValue);
		}
		if (myTag != 0 ){
      myFile.setTag(*myTag);
			retVal = FS_HAL::save(myFile);
		}
	}
	return retVal;
}

std::string Command::getTypeName(int iType){
	std::string retVal = "tag_type_not_found";
	switch (iType){
	case 0:
		retVal = "bool";
		break;
	case 1:
		retVal = "number";
		break;
	case 2:
		retVal = "text";
		break;
	}
	return retVal;
}