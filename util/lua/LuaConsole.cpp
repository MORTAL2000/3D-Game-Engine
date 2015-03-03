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
