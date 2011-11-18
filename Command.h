// Command.h: Schnittstelle für die Klasse command.
//
// Command will be a small wrapper for the commandline functions 
// which will be executed in this class and are only a base of
// basic tasks.
// This is a list of what is implemented:
//		meta_tree get(filename, 
//									[TAGLIST]) :				prints all tags or the given tags
//																			from a file/directory (which means 
//																			all files in this directory) which
//																			have these tags
//
//		meta_tree set(filename, 
//									tagname, 
//									[tagvalue], 
//									[tagtype])          sets a tag to this file (or all 
//																			files in this directory) and sets
//																			to a given value
//																			if tagvalue is not set, it will 
//																			be a boolean tag,
//																			if tagtype is not set it is 
//																			assumed a_text type
//
//		meta_tree rm/del(	filename, 
//											tagname, 
//											tagname, [..])	deletes list of tagnames from a 
//																			given file or directory 
//																			If no tagname is given ALL tags 
//																			will be removed!
//		meta_tree ls/list(	filename,
//												filename,[..])lists all tags from all files in 
//																			this list (difference to get: 
//																			no TAGLIST, so get without taglist is same)
//
//
//		meta_tree search/filter( dirname, 
//											tagname[="tagvalue"],
//											tagname,[..])		will output only files which have all
//																			tags set (may be to a given value)
//		meta_tree sort(	src_dir, 
//										dst_dir, 
//										tagname[="tagvalue"], <-- vec<pair<std::string, std::string>>
//										tagname,[...])    will hardlink all files from src_dir
//																			to a new structure in dst_dir with 
//																			the tags as folders and all files 
//																			with matching tags inside this folders
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMAND_H__78660288_A1F5_4452_98ED_A5879B9602FF__INCLUDED_)
#define AFX_COMMAND_H__78660288_A1F5_4452_98ED_A5879B9602FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include "app.h"


namespace APP_NAME{
	class Command  
	{
	public:
		static int del(std::string FileName, std::string TagName);
		static int get(std::string FileName, std::vector<std::string> TagList = std::vector<std::string>());
		static int set(std::string FileName, std::string TagName, std::string TagValue="");
		static int set(std::string FileName, std::string TagName, std::string TagValue, std::string TagType);

		static int list(std::vector<std::string> Filenames);
    static int sort (std::string src_dir, std::string dst_dir, std::vector<std::pair<std::string, std::string*> > tags);
    static int search (std::string src_dir, std::vector<std::pair<std::string, std::string*> > tags);

		static std::string getTypeName(int iType);
		static std::vector<std::string> collectFiles(std::string  strDir, std::string strPattern="*", 
			                                           std::vector<std::string>retVal=std::vector<std::string>(),
																								 bool recurse=true);
		static int wildcmp(const char *wild, const char *string) ;
	};
}

#endif // !defined(AFX_COMMAND_H__78660288_A1F5_4452_98ED_A5879B9602FF__INCLUDED_)
