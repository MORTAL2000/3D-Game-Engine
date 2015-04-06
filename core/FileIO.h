#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

#include "Console.h"

namespace FileIO
{

	bool is_dir(const char*);
	bool is_file(const char*);

	bool inDir(const std::string& path, std::string& dir);
	bool getTopDir(const std::string& path, std::string& dir);

	std::vector<std::string> getFilesInDirectory(const std::string&);
	std::vector<std::string> getFilesInWorkingDirectory();

	void setCurrentDirectory(const std::string&);
	std::string getCurrentDirectory();

	bool createDirectory(const std::string&);
	std::string browseFolder();
	std::string browseFile(const char*, const char*);
	std::string saveFile();
}

#endif
