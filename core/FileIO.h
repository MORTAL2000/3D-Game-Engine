/*
 * Copyright 2015 Alexander Koch
 * File: FileIO.h
 * Description: Methods for file operations.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
