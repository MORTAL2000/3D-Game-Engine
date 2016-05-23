/*
 * Copyright 2015 Alexander Koch
 * File: PropertyParser.cpp
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

#include "PropertyParser.h"

PropertyParser::PropertyParser() {
	m_cache["width"] = "1280";
	m_cache["height"] = "720";
	m_cache["hideCursor"] = "1";
}

PropertyParser& PropertyParser::getInstance() {
	static PropertyParser instance;
	return instance;
}

bool PropertyParser::load(const string& filename) {
	vector<string> lines = FileReader::readLines(filename);

	for(size_t i = 0; i < lines.size(); i++) {
		size_t comment = lines[i].find('#');
		if(comment != string::npos) lines[i].erase(comment);
		if(lines[i].size() == 0) continue;

		lines[i] = Tokenizer::removeWhitespace(lines[i]);
		vector<string> values = Tokenizer::tokenize(lines[i], '=');
		m_cache[values[0]] = values[1];
	}

	return true;
}

// access methods
void PropertyParser::getProperty(const string& key, bool& value) {
	int v;
	sscanf(m_cache[key].c_str(), "%i", &v);
	value = (v > 0);
}

void PropertyParser::getProperty(const string& key, float& value) {
	sscanf(m_cache[key].c_str(), "%f", &value);
}

void PropertyParser::getProperty(const string& key, int& value) {
	sscanf(m_cache[key].c_str(), "%i", &value);
}

void PropertyParser::getProperty(const string& key, string& value) {
	value = m_cache[key];
}

void PropertyParser::printCache() {
	Console::log("------ Properties ------");
	for(PropertiesCache::iterator it = m_cache.begin(); it != m_cache.end(); it++)
	Console::log("%s = %s", it->first.c_str(), it->second.c_str());
	Console::log("------ ---------- ------");
}
