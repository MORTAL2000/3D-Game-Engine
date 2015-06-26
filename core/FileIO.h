#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>
using std::string;
using std::vector;

#include <dir.h>
#include <direct.h>
#include "Console.h"

namespace FileIO
{

	bool is_dir(const char*);
	bool is_file(const char*);

	bool inDir(const string& path, string& dir);
	bool getTopDir(const string& path, string& dir);

	vector<string> getFilesInDirectory(const string&);
	vector<string> getFilesInWorkingDirectory();

	void setCurrentDirectory(const string&);
	string getCurrentDirectory();

	bool createDirectory(const string&);
	string browseFolder();
	string browseFile(const char*, const char*);
	string saveFile();
}

#endif
