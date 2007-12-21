// bucket.h: Schnittstelle für die Klasse bucket.
//
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

	class Bucket  
	{
	private:
	public:
		std::string strName;
		std::map<std::string, void*> folder;
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
