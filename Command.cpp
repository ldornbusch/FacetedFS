// command.cpp: Implementierung der Klasse command.
//
//////////////////////////////////////////////////////////////////////////////
#pragma warning( disable :  4786 )

#include <stdio.h>
#include <sstream>

#include <dirent.h>

#include "Command.h"

#include "FS_HAL.h"
#include "bucket.h"
#include "NumberTag.h"
#include "StringTag.h"

using namespace APP_NAME;

//////////////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////////////
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
			printf(outputstring.c_str());			// TODO: use sdtout stream as well is very UGLY!!
			boolean first = true;
			for (it = myFile.map_FileTags.begin(); 
				   it != myFile.map_FileTags.end();
					 ++it){
				if (!first){
					printf("\n     --- separator ---\n");
				}else{
					printf("\n");
				}
				outputstream.clear();
				outputstream << it->first << "[" << 
					Command::getTypeName(it->second->getType()) << "]:" ;
				outputstream >> outputstring;
				printf(outputstring.c_str());
				outputstream.clear();
				if (it->second->getType()>0){
					printf("\n");
					outputstream << it->second->getStringValue();
					outputstream >> outputstring;
					printf(outputstring.c_str());
				}
				first = false;		// output separator before output
			}
			printf("\n     --- END ---\n");
		}	else{
			retVal=5; // TODO:UNIFY ERROR CODES   This one means: TAGS NOT MATCHING
		}
	}
	return retVal;
}

int Command::set(std::string FileName, 
								 std::string TagName, 
								 std::string TagValue){
	int retVal = 0;
	if (TagValue.length() == 0){
		retVal = Command::set(FileName, TagName, TagValue, "BOOL");
	} else{
		retVal = Command::set(FileName, TagName, TagValue, "TEXT");
	}
	return retVal;
}

int Command::set(std::string FileName, 
								 std::string TagName, 
								 std::string TagValue, 
								 std::string TagType){
	int retVal = 0;
	File myFile(FileName);
	retVal = FS_HAL::load(myFile, std::vector<std::string>());
	if (retVal == 0 || retVal == FILE_EMPTY || retVal == FILE_NOT_FOUND){
    Tag * myTag = 0;
		retVal = 6 ;  
		// TODO:UNIFY ERROR CODES   This one means: TAGTYPE NOT IMPLEMENTED
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

int Command::del(std::string FileName, std::string TagName){
	int retVal = 0;
	File myFile(FileName);
	retVal = FS_HAL::load(myFile, std::vector<std::string>());
	if (retVal == 0){
		myFile.removeTag(TagName);
		FS_HAL::save(myFile);
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

int Command::list(std::vector<std::string> Filenames){
	int retVal = FILE_OK;
	std::vector<std::string>::const_iterator it;
	for (it = Filenames.begin(); it != Filenames.end(); ++it){
		retVal += Command::get(*it);
	}
	return retVal;
}

int Command::sort (std::string src_dir, std::string dst_dir, std::vector<std::pair<std::string, std::string*> > tags){
	return FILE_OK;
}

int Command::search (std::string src_dir, std::vector<std::pair<std::string, std::string*> > tags){
	return FILE_OK;
}


std::vector<std::string> Command::collectFiles(std::string strDir, std::string strPattern, std::vector<std::string>retVal,bool recurse){
	struct dirent *ent;
  DIR *dir= opendir (strDir.c_str());
  if (dir != 0) {
    /* traverse all the files and directories within directory */
		std::string strFullName ;
		std::string strType ;
		bool isDir ;
	  while ((ent = readdir (dir)) != NULL) {
			if (strcmp(ent->d_name, ".") !=0 &&  strcmp(ent->d_name, "..") !=0){
				strFullName = strDir +"\\"+std::string(ent->d_name);
				isDir = 	(ent->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) !=0;
				int match = Command::wildcmp(strPattern.c_str(), ent->d_name) ;
				if (match && (!isDir)){
					retVal.push_back(strFullName);
				}	
				if (isDir && recurse){
					retVal = Command::collectFiles(strFullName, strPattern,retVal, recurse);
				}
			}
		}
    closedir (dir);
		return retVal;
  } else {
    /* could not open directory */
    perror ("DIR NOT FOUND!");
    return retVal;
  }
}



// patternmatch function for globbing filenames with windows wildcards:
// thanx to   Jack Handy - jakkhandy@hotmail.com
// found here: http://www.codeproject.com/KB/string/wildcmp.aspx
/*
	if (wildcmp("bl?h*e", "blah.exg")) {
		std::cout << "we have a match!\n";
	} else {
		std::cout << "  no match =(\n";
	}
*/
int Command::wildcmp(const char *wild, const char *string) {
  // Written by Jack Handy - jakkhandy@hotmail.com

  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}
