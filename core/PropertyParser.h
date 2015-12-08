/**
 * @file PropertyParser.h
 * @author Alexander Koch
 * @desc Parses .properties files
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
