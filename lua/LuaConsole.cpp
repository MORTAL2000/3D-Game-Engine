/*
 * Copyright 2015 Alexander Koch
 * File: LuaConsole.cpp
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

#include "LuaConsole.h"

namespace LuaConsole
{
	lua_Debug ar;

	void error(lua_State* lua, const char* format, ...)
	{
		char buffer[256];
		va_list args;
		va_start(args, format);
		vsprintf(buffer, format, args);
		va_end(args);

		lua_getstack(lua, 1, &ar);
		lua_getinfo(lua, "nSl", &ar);

		Console::log("%s (line : %d)", buffer, ar.currentline);
	}
}
