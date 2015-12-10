/*
 * Copyright 2015 Alexander Koch
 * File: PropertyParser.h
 * Description: Parses *.properties files.
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

#ifndef PROPERTYPARSER_H
#define PROPERTYPARSER_H

#include <map>
#include <string>
#include <vector>
using std::string;
using std::map;
using std::vector;

#include <stdio.h>
#include <stdlib.h>

#include "Tokenizer.h"
#include "FileReader.h"
#include "Console.h"

typedef map<string, string> PropertiesCache;

class PropertyParser
{
public:
	static PropertyParser& getInstance();

	bool load(const string&);
	void getProperty(const string&, bool&);
	void getProperty(const string&, int&);
	void getProperty(const string&, float&);
	void getProperty(const string&, string&);

	void printCache();
private:
	PropertiesCache m_cache;
	PropertyParser();
};

#endif
