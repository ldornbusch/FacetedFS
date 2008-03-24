// meta_tree_cmd.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//
#pragma warning( disable :  4786 )

#include <stdio.h>
#include <vector>

#include "FS_HAL.h"
#include "bucket.h"
#include "NumberTag.h"
#include "StringTag.h"



int main(int argc, char* argv[])
{
	using namespace APP_NAME;

	// testing load/save of tags
	std::string filename="c:\\LinkSrc.txt";
	File testFile(filename);
	testFile.setTag(Tag("BOOLTAG"));
  testFile.setTag(*(new NumberTag("PI",3.14)));
  testFile.setTag(*(new StringTag("Hallo","Welt")));
	FS_HAL::load(testFile);
	FS_HAL::save(testFile);

	printf("\n");
	// testing the creation of a File Link
	DWORD result ;
	result = FS_HAL::createHardLink("c:\\LinkDest.txt",filename);
	result = FS_HAL::createHardLink("c:\\LinkDest2.txt","c:\\LinkDest.txt");
	FS_HAL::unload();

	// testing a bucket tag-sort:
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

