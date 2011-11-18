// bucket.h: Interface for class Bucket.
// A bucket is a group of FILEes with the setting of a certain Tag
// Buckets are nested in an initial Tag sequence.
// The TagName which all files should have is also the Name of the Bucket
// For each File which has the certain Tag, its value will be created
// in the map "folder" as new Bucket. this newly created bucket gets the 
// next name in the tags2sort chain. Also the file will be recursively 
// added to the new (or found) bucket.
// At the end there is a tree structure consisting of all Tagnames and 
// their respective values as hierachy in the buckets.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUCKET_H__C2F487EC_05B1_4214_A140_600B20B34FE6__INCLUDED_)
#define AFX_BUCKET_H__C2F487EC_05B1_4214_A140_600B20B34FE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include <map>

#include "app.h"
#include "file.h"


namespace APP_NAME{
class Bucket;

	class Bucket  
	{
	private:
	public:
		std::string strName;
		std::map<std::string, Bucket*> folder;
		std::vector<File> vecNoMatch;
		std::vector<Tag> vecTags2Match; 
	public:
		Bucket();
		virtual ~Bucket();

		void init(std::vector<Tag> tags2sort);
		
		int getFileNumber();
		void addFileToBucket(const APP_NAME::File newFile);
		bool hasValue(const std::string value);	
		void debug_print(const std::string preset="");
	};
}

#endif // !defined(AFX_BUCKET_H__C2F487EC_05B1_4214_A140_600B20B34FE6__INCLUDED_)
