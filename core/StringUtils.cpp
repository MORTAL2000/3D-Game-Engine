/*
 * Copyright 2015 Alexander Koch
 * File: StringUtils.cpp
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

#include "StringUtils.h"

namespace StringUtils
{

	string format_str(const char* format, ...)
	{
		char buffer[256];
		va_list args;
		va_start(args, format);
		vsprintf(buffer, format, args);
		va_end(args);
		return std::string(buffer);
	}

	// lower case + replace whitespace by underscore
	string simplify(const std::string& source)
	{
		string temp = source;
		std::replace_if(temp.begin(), temp.end(), isspace, '_');
		return toLower(temp);
	}

	string toLower(const std::string& str)
	{
		string data = str;
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}

}
