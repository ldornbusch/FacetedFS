// bucket.cpp: Implementierung der Klasse bucket.
//
//////////////////////////////////////////////////////////////////////

#include "bucket.h"
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
using namespace APP_NAME;

Bucket::Bucket()
{
}

Bucket::~Bucket()
{
}

void Bucket::init(const std::vector<Tag> tags2sort){
	Tag topTag;
	if (tags2sort.size()>0){
		topTag = tags2sort[tags2sort.size()-1];
		this->vecTags2Match = tags2sort;
		this->vecTags2Match.pop_back();
	}
	this->strName = topTag.getTagname();		// this name will be "" and so we know this is the last tag in the folder chain so all values which has all other tags will be ste into these buckets into the matched folder
}

int Bucket::getFileNumber(){
	return -1;
}

bool Bucket::hasValue(const std::string value){
	return (folder.find(value) != folder.end());
}

/** will sort file into the apropriate bucket chain through recursion.
		returns if the file was filled into the bucket.
		The caller-bucket who receives a false-return, will put it into 
		his own unmatched folder and returns true(as finally file was sorted in)
*/
void Bucket::addFileToBucket(APP_NAME::File newFile){
	// check if File has this Tag..
	if (this->strName.length()>0 ){		// last folderchain: put file into own match-vector
		if (newFile.hasTag(this->strName)){
			std::string val = newFile.getTag(this->strName).getStringValue();

			// check if already a bucket with this tag-value exists
			Bucket* ptrBucket = 0; 
			if (this->hasValue(val)){
				ptrBucket = (Bucket*)this->folder[val];
			}else{	// create new bucket with this value, put its adress into the folder and recurse
				ptrBucket = new Bucket();
				ptrBucket->init(this->vecTags2Match);
				this->folder[val]=ptrBucket;
			}
			ptrBucket->addFileToBucket(newFile);
		}else{
			this->vecNoMatch.push_back(newFile);
		}
	}else{
		this->vecNoMatch.push_back(newFile);
	}
}

void Bucket::debug_print(){
	printf(this->strName.c_str());
	printf(":\n");
	for (std::map<std::string, void*>::iterator it1 = folder.begin(); it1!= folder.end(); it1++){
		std::string strBucketVal = (*it1).first;
		Bucket* tmp_folder= (Bucket*)(*it1).second;
		printf(strBucketVal.c_str());
		printf(":\n");
		tmp_folder->debug_print();
		printf("-----unmatched of ");
		printf(this->strName.c_str());
		printf(":\n");
	}
	for (std::vector<File>::iterator it2 = vecNoMatch.begin(); it2!= vecNoMatch.end(); it2++){
		printf((*it2).file_name.c_str());
		printf("\n");
	}
		printf("-----end of ");
		printf(this->strName.c_str());
		printf(":\n");
		
}