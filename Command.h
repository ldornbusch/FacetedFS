// Command.h: Schnittstelle für die Klasse command.
//
// Command will be a small wrapper for the commandline functions 
// which will be executed in this class and are only a base of
// basic tasks.
// This is a list of what is implemented:
//		meta_tree get(filename, 
//									[TAGLIST]) :				retrieve all tags or the given tags
//																			from a file/directory (which means 
//																			all files in this directory)
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
//																			this list
//
//
//		meta_tree search( dirname, 
//											tagname[="tagvalue"],
//											tagname,[..])		will output only files which have all
//																			tags set (to a given value)
//		meta_tree sort(	src_dir, 
//										dst_dir, 
//										tagname[="tagvalue"],
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
		static std::string getTypeName(int iType);
	};
}

#endif // !defined(AFX_COMMAND_H__78660288_A1F5_4452_98ED_A5879B9602FF__INCLUDED_)
