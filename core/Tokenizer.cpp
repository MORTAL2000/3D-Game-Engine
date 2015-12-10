/*
 * Copyright 2015 Alexander Koch
 * File: Tokenizer.cpp
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

#include "Tokenizer.h"

namespace Tokenizer
{
	vector<string> tokenize(string str, char delimiter)
	{
		vector<string> values;
		size_t delimiters = 0, j, i = 0;
		for(j = 0; j < str.size(); j++) if(str[j] == delimiter) delimiters++;
		if(delimiters == 0) return vector<string>();
		for(j = 0; j < delimiters; j++)
		{
			string value;
			while(str[i] != delimiter && i < str.size())
			{
				value += str[i];
				i++;
			}
			i++;
			values.push_back(value);
		}

		string lastVal;
		for(j = i; j < str.size(); j++) lastVal += str[j];
		values.push_back(lastVal);
		return values;
	}

	/**
	 *	Returns extension without the dot
	 */
	string getFileExtension(const string& name)
	{
		auto found = name.find_last_of(".");
		if(found == (unsigned)-1) return string();
		return name.substr(found+1);
	}

	string getDirectory(const string& path)
	{
		return getDirectory(path, false);
	}

	/**
	 *	Removes last directory from file / the file
	 *  slash -> add '/' to the end
	 */
	string getDirectory(const string& path, bool slash)
	{
		auto found = path.find_last_of("\\/");
		if(found == (unsigned)-1) return string();
		return path.substr(0, found + (slash? 1 : 0));
	}

	string removePath(const string& name)
	{
	 	auto found = name.find_last_of("\\/");
		if(found == (unsigned)-1) return name;
		return name.substr(found+1);
	}

	string removeWhitespace(const string& source)
	{
		string out;
		for(auto i = 0; i < source.size(); i++)
		{
			if(!std::isspace(source[i]))
			{
				out += source[i];
			}
		}
		return out;
	}

	vector<string> parseLines(const string& source)
	{
		vector<string> lines;
		string current;
		for(auto i = 0; i < source.size(); i++)
		{
			if(source[i] == '\r') continue;
			if(source[i] == '\n') {
				lines.push_back(current);
				current.clear();
			}
			else {
				current += source[i];
			}

			if(i == source.size()-1) lines.push_back(current); //last line without '\n' should be uploaded
		}
		return lines;
	}

	string removeCharacter(const string& source, char ch)
	{
		string result;
		for(auto t = 0; t < source.size(); t++)
		{
			if(source[t] != ch)
			result += source[t];
		}
		return result;
	}

	vector<std::size_t> findAll(const string& source, const string& expression)
	{
		vector<std::size_t> positions;
		size_t found = source.find(expression, 0);
		while(found != string::npos)
		{
		    positions.push_back(found);
		    found = source.find(expression,found+1);
		}
		return positions;
	}

	vector<std::size_t> findAllDigits(const string& source)
	{
		vector<std::size_t> positions;
		for(auto i = 0; i < source.size(); i++)
		{
			if(isdigit(source[i]))
			{
				positions.push_back(i);
			}
		}
		return positions;
	}
}
