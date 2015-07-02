#ifndef LUA_ENUMS_H
#define LUA_ENUMS_H

#include <iostream>
#include <sstream>

#include "LuaBase.h"

bool add_enum_to_lua(lua_State*, const char*, ...);
bool check_enum_type(lua_State*, const char*, int);
int get_enum_value(lua_State*, int);

#endif
