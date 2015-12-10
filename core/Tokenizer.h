/*
 * Copyright 2015 Alexander Koch
 * File: Tokenizer.h
 * Description: General purpose functions for strings.
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

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <sstream>
#include <string>
#include <vector>
#include <ctype.h>
using vector;
using string;

namespace Tokenizer
{
	vector<string> tokenize(string, char);
	string getFileExtension(const string&);
	string getDirectory(const string&);
	string getDirectory(const string&, bool);
	string removePath(const string&);
	string removeWhitespace(const string&);
	vector<string> parseLines(const string&);
	string removeCharacter(const string&, char);
	vector<std::size_t> findAll(const string&, const string&);
	vector<std::size_t> findAllDigits(const string&);
}

#endif
