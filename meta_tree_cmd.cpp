// meta_tree_cmd.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//
#pragma warning( disable :  4786 )

#include <stdio.h>
#include <vector>

#include <dirent.h>

#include "FS_HAL.h"
#include "bucket.h"
#include "NumberTag.h"
#include "StringTag.h"
#include "Command.h"


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
int wildcmp(const char *wild, const char *string) {
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

int testPrintDir(std::string  strDir, std::string strPattern="*"){
  struct dirent *ent;
  DIR *dir;
	dir = opendir (strDir.c_str());

  if (dir != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
			if (wildcmp(strPattern.c_str(), ent->d_name)){
				printf ("%s\n", ent->d_name);
			}	
    }
    closedir (dir);
		return EXIT_SUCCESS;
  } else {
    /* could not open directory */
    perror ("DIR NOT FOUND!");
    return EXIT_FAILURE;
  }
}

using namespace APP_NAME;

void testGetSetDel(std::string filename){
		std::vector<std::string> vec;
//	vec.push_back("Hallo");
//	vec.push_back("PI");
//	Command::get(filename, vec);
	Command::set(filename,"Lollipop2");
	Command::set(filename,"LolliCount","4.322","NUMBER");
	Command::set(filename,"LolliName", "Lutz");
 	Command::get(filename, vec);
	Command::del(filename,"Lollipop2");
 	Command::get(filename);
}

void testLoadSave(std::string filename){	// testing load/save of tags
	File testFile(filename);
	testFile.setTag(Tag("BOOLTAG"));
	testFile.setTag(*(new NumberTag("PI",3.141592)));
	testFile.setTag(*(new StringTag("Hallo","Lutz")));
	FS_HAL::load(testFile);
	FS_HAL::save(testFile);
}

void testHardLinkCreate(std::string filename){ 	
	// testing the creation of a File Link
	DWORD result ;
	result = FS_HAL::createHardLink("c:\\metatree\\LinkDest.txt",filename);
	result = FS_HAL::createHardLink("c:\\metatree\\LinkDest2.txt",
	                                       "c:\\metatree\\LinkDest.txt");
	FS_HAL::unload();
}

void testBucketSort(){	// testing a bucket tag-sort:
	const std::string tag1="1stName", tag2="2ndName", tag3="3rdName";

	std::vector<Tag> vec_SortTags;
	vec_SortTags.push_back(tag3);
	vec_SortTags.push_back(tag2);
	vec_SortTags.push_back(tag1);

	File myFileA("FileA"), myFileB("FileB"), myFileC("FileC"), myFileD("FileD");
	File myFileE("FileE"), myFileF("FileF"), myFileG("FileG"), myFileH("FileH");
	File myFileI("FileI"), myFileJ("FileJ"), myFileK("FileK"), myFileL("FileL");
	myFileA.setTag(Tag(tag1));
	myFileA.setTag(Tag(tag2));
	myFileA.setTag(Tag(tag3));
	myFileB.setTag(Tag(tag1));
	myFileB.setTag(Tag(tag2));
	myFileC.setTag(Tag(tag1));

	myFileE.setTag(*(new NumberTag(tag1,1.0)));
	myFileF.setTag(*new NumberTag(tag1,2.0));
	myFileG.setTag(*new NumberTag(tag1,3.0));
	myFileH.setTag(*new NumberTag(tag2,1.0));

	myFileI.setTag(*(new StringTag(tag1,"superDuper")));
	myFileI.setTag(*(new StringTag(tag2,"nett")));
	myFileI.setTag(*(new StringTag(tag3,"cool")));
	myFileJ.setTag(*new StringTag(tag1,"Mega Maessig"));
	myFileJ.setTag(*(new StringTag(tag2,"auch nett")));
	myFileK.setTag(*new StringTag(tag1,"superDuper"));
	myFileL.setTag(*new StringTag(tag2,"genial"));

	Bucket rootBucket;
	rootBucket.init(vec_SortTags);

//	Tag* n = &(myFileE.getTag(tag1));
//	std::string s = n->getStringValue();

	rootBucket.addFileToBucket(myFileA);
	rootBucket.addFileToBucket(myFileB);
	rootBucket.addFileToBucket(myFileC);
	rootBucket.addFileToBucket(myFileD);
	rootBucket.addFileToBucket(myFileE);
	rootBucket.addFileToBucket(myFileF);
	rootBucket.addFileToBucket(myFileG);
	rootBucket.addFileToBucket(myFileH);
	rootBucket.addFileToBucket(myFileI);
	rootBucket.addFileToBucket(myFileJ);
	rootBucket.addFileToBucket(myFileK);
	rootBucket.addFileToBucket(myFileL);

	rootBucket.debug_print("--");
}




int main(int argc, char* argv[])
{
	testPrintDir("c:\\metatree\\test_src", "*.txt");

	testGetSetDel("c:\\metatree\\TagTest.txt");

	std::string filename="c:\\metatree\\LinkSrc.txt";

	testLoadSave(filename);

	testHardLinkCreate(filename);	

	testBucketSort();

	return 0;
}

