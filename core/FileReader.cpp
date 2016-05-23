/*
 * Copyright 2015 Alexander Koch
 * File: FileReader.cpp
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

#include "FileReader.h"
#include "Console.h"

namespace FileReader {
	std::string read(const std::string& filename) {
		std::ifstream t(filename);
		std::string str((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());
		return str;
	}

	std::vector<std::string> readLines(const std::string& filename) {
		std::vector<std::string> lines;

		std::ifstream file(filename);
	    std::string str;
	    while(std::getline(file, str)) {
			lines.push_back(str);
	    }
		return lines;
	}
}
