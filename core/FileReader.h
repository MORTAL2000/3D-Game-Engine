/*
 * Copyright 2015 Alexander Koch
 * File: FileReader.h
 * Description: Useful methods for reading files.
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

#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <vector>
#include <fstream>
#include <streambuf>

#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include "Tokenizer.h"
#include "FileIO.h"

namespace FileReader {
	//	Reads given file
	std::string read(const std::string& filename);

	// Read all lines into a vector
	std::vector<std::string> readLines(const std::string&);
}

#endif
