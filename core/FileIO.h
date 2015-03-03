#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

#include "Console.h"

namespace FileIO
{

	bool isFile(const std::string&);
	std::vector<std::string> getFilesInDirectory(const std::string&);
	std::vector<std::string> getFilesInWorkingDirectory();

	void setCurrentDirectory(const std::string&);
	std::string getCurrentDirectory();

	bool createDirectory(const std::string&);
	std::string browseFolder();
	std::string browseFile(const char*, const char*);
}

#endif
