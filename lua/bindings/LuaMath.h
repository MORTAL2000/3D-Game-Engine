#ifndef LUA_MATH_H
#define LUA_MATH_H

#include <vector>

#include "../LuaBase.h"
#include "../LuaClass.h"
#include "../LuaConsole.h"


// main functions
void lua_push_vector(lua_State* lua, const float* array, int size);
std::vector<float> lua_read_vector(lua_State* lua, int index);

// wrappers

void lua_push_vec3(lua_State* lua, const vec3& table);
void lua_push_vec4(lua_State* lua, const vec4& table);
vec3 lua_read_vec3(lua_State* lua, int index);
vec4 lua_read_vec4(lua_State* lua, int index);

namespace LuaMath
{

// operators

int sub(lua_State* lua);
int add(lua_State* lua);
int dot(lua_State* lua);
int cross(lua_State* lua);
int length(lua_State* lua);
int normalize(lua_State* lua);
int clamp(lua_State* lua);

}

#endif
