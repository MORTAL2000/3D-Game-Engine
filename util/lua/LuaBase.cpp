#include "LuaBase.h"

void lua_clean(lua_State* lua)
{
	int n = lua_gettop(lua);
	lua_pop(lua, n);
	lua_settop(lua, 0);
}
