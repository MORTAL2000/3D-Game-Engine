/**
 * @file PropertyParser.h
 * @author Alexander Koch
 * @desc parses .properties files
 */

#ifndef PROPERTYPARSER_H
#define PROPERTYPARSER_H

#include <map>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include "Tokenizer.h"
#include "FileReader.h"
#include "Console.h"

typedef std::map<std::string, std::string> PropertiesCache;

class PropertyParser
{
public:
	static PropertyParser& getInstance();

	bool load(const std::string&);
	void getProperty(const std::string&, bool&);
	void getProperty(const std::string&, int&);
	void getProperty(const std::string&, float&);
	void getProperty(const std::string&, std::string&);

	void printCache();
private:
	PropertiesCache m_cache;
	PropertyParser();
};

#endif
