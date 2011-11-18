// meta_tree_cmd.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//
#pragma warning( disable :  4786 )
#pragma comment( lib, "winmm" )

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>

#include <dirent.h>

#include "FS_HAL.h"
#include "bucket.h"
#include "NumberTag.h"
#include "StringTag.h"
#include "Command.h"

using namespace APP_NAME;


int testPrintDir(std::string  strDir, std::string strPattern="*", bool recurse=true){
	return Command::collectFiles(strDir,strPattern, std::vector<std::string>(),recurse).size();
}


int testPrintDir_OLD(std::string  strDir, std::string strPattern="*", bool recurse=true){
  struct dirent *ent;
  DIR *dir;
	dir = opendir (strDir.c_str());
	int retVal = 0;
  if (dir != 0) {
    /* print all the files and directories within directory */
		std::string strFullName ;
		std::string strType ;
		bool isDir ;
	  while ((ent = readdir (dir)) != NULL) {
			// use stat(path+file) to check for dir or file
			if (strcmp(ent->d_name, ".") !=0 &&  strcmp(ent->d_name, "..") !=0){
				strFullName = strDir +"\\"+std::string(ent->d_name);
				strType = "N/A";
/*
				// this is more compatible, however it needs an additional function call
				struct stat statData;
				int result = stat(strFullName.c_str(),&statData);
				if (result == 0){
					strType = (statData.st_mode & _S_IFDIR)?"DIR":"FILE";
				}
				if (wildcmp(strPattern.c_str(), ent->d_name) || (statData.st_mode & _S_IFDIR)){
					printf ("%s <%s>\n", strFullName.c_str(),strType.c_str());//ent->d_name);
				}	
*/
				// this is now win32 only!
				isDir = 	(ent->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) !=0;
				strType = (isDir)?"DIR":"FILE";		
				int match = 0;//Command::wildcmp(strPattern.c_str(), ent->d_name) ;
				if (match && (!isDir)){
				//if ((!isDir)){
					printf ("%s <%s>\n", strFullName.c_str(),strType.c_str());//ent->d_name);
					retVal++;
				}	
				if (isDir && recurse){
					retVal += testPrintDir(strFullName, strPattern, recurse);
				}
			}
		}
    closedir (dir);
		return retVal;
  } else {
    /* could not open directory */
    perror ("DIR NOT FOUND!");
    return -1;
  }
}


int testGetSetDel(std::string filename){
		std::vector<std::string> vec;
//	vec.push_back("Hallo");
//	vec.push_back("PI");
//	Command::get(filename, vec);
	DWORD result ;
	result = Command::set(filename,"Lollipop2");
	result += Command::set(filename,"LolliCount","4.322","NUMBER");
	result += Command::set(filename,"LolliName", "Lutz");
 	result += Command::get(filename, vec);
	result += Command::del(filename,"Lollipop2");
 	result += Command::get(filename);
	return result;
}

int testLoadSave(std::string filename){	// testing load/save of tags
	DWORD result ;
	File testFile(filename);
	result = FS_HAL::load(testFile);
	testFile.setTag(Tag("BOOLTAG"));

	NumberTag initial=NumberTag("increment",1.0);
  NumberTag* tmpTag = ((NumberTag*)(&(testFile.getTag("increment"))));
	if (testFile.hasTag("increment")){
		tmpTag->setValue(tmpTag->getValue()+1.0);
	}else{
		tmpTag=&initial;
	}
	testFile.setTag(*tmpTag);

	testFile.setTag(*(new NumberTag("PI",3.141592)));
	testFile.setTag(*(new StringTag("Hallo","Lutz")));
	result += FS_HAL::save(testFile);
	return result;
}

int testHardLinkCreate(std::string filename){ 	
	// testing the creation of a File Link
	DWORD result ;
	result = FS_HAL::createHardLink("c:\\meta_tree\\test_dst\\LinkDest.txt",filename);
	result += FS_HAL::createHardLink("c:\\meta_tree\\test_dst\\LinkDest2.txt",
                                  "c:\\meta_tree\\test_dst\\LinkDest.txt");
	FS_HAL::unload();
	return result;
}

int testBucketSort(){	// testing a bucket tag-sort:
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
	return 0;
}

int testCreateDir(std::string dirname){
	return FS_HAL::createDirectory(dirname);
}


int main(int argc, char* argv[])
{
	DWORD start = timeGetTime(); 

	int result = 0;//testPrintDir("c:\\meta_tree\\test_src", "*.h");

	DWORD end = timeGetTime(); 

	printf ("%d files found in %d milliseconds\n",result, (end-start));

	std::string filename="c:\\meta_tree\\LinkSrc.txt";

	result = testLoadSave(filename);
/*

	result = testGetSetDel("c:\\meta_tree\\TagTest.txt");

	testCreateDir("c:\\meta_tree\\test_dst");

	result = testHardLinkCreate(filename);	

	result = Command::get("c:\\meta_tree\\DoesNotExists.txt");

  result = testBucketSort();
*/
	Command::list(Command::collectFiles("c:\\meta_tree\\", "link*.txt",std::vector<std::string>(), false));

	return result;
}

