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
				outputstream << it->first << "[" << it->second->getType() << "]:" ;
				outputstream >> outputstring;
				printf(outputstring.c_str());
				printf("\n");
        outputstream.clear();
				outputstream << it->second->getStringValue();
				outputstream >> outputstring;
				printf(outputstring.c_str());
				printf("\n\n");
			}
		}	else{
			retVal=5; // TODO:UNIFY ERROR CODES   This one means: TAGS NOT MATCHING
		}
	}
	return retVal;
}