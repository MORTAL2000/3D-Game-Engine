/*
 * Copyright 2015 Alexander Koch
 * File: Console.cpp
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

#include "Console.h"

namespace Console
{
	bool active = true;

	void setActive(bool status)
	{
		active = status;
	}

	void logRaw(const std::string& message)
	{
		if(active)
		{
#ifdef __ANDROID_API__
			LOGI("%s\n", message.c_str());
#else
			std::cout << message << std::endl;
#endif
		}
	}

	void log(const char* format, ...)
	{
		char buffer[256];
		va_list args;
		va_start(args, format);
		vsprintf(buffer, format, args);
		va_end(args);
		logRaw(std::string(buffer));
	}

	void logif(bool condition, const char* format, ...)
	{
		if(condition)
		{
			char buffer[256];
			va_list args;
			va_start(args, format);
			vsprintf(buffer, format, args);
			va_end(args);
			logRaw(std::string(buffer));
		}
	}

	void newline()
	{
#ifdef __ANDROID_API__
		LOGI("\n");
#else
		std::cout << '\n';
#endif
	}

	void logVector(const std::vector<std::string>& list)
	{
		if(list.size() > 0)
		{
			std::cout << "[" << list[0];
			for(auto i = 1; i < list.size(); i++)
			{
				std::cout << ", " << list[i];
			}
			std::cout << "]" << std::endl;
		}
	}

	void logVec3(const vec3& vector)
	{
		log("[%f, %f, %f]", vector.x, vector.y, vector.z);
	}
}
