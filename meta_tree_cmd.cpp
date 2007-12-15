// meta_tree_cmd.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//
#include <stdio.h>
#include <vector>

#include "bucket.h"

int main(int argc, char* argv[])
{
	using namespace APP_NAME;
	const std::string tag1="1stName", tag2="2ndName", tag3="3rdName";

	std::vector<Tag> vec_SortTags;
	vec_SortTags.push_back(tag3);
	vec_SortTags.push_back(tag2);
	vec_SortTags.push_back(tag1);

	File myFileA("FileA"), myFileB("FileB"), myFileC("FileC"), myFileD("FileD");
	myFileA.setTag(Tag(tag1));
	myFileA.setTag(Tag(tag2));
	myFileA.setTag(Tag(tag3));
	myFileB.setTag(Tag(tag1));
	myFileB.setTag(Tag(tag2));
	myFileC.setTag(Tag(tag1));

	Bucket rootBucket;
	rootBucket.init(vec_SortTags);
	rootBucket.addFileToBucket(myFileA);
	rootBucket.addFileToBucket(myFileB);
	rootBucket.addFileToBucket(myFileC);
	rootBucket.addFileToBucket(myFileD);

	rootBucket.debug_print();

	return 0;
}

