/*
 * Copyright 2015 Alexander Koch
 * File: LuaMath.h
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

#ifndef LUA_MATH_H
#define LUA_MATH_H

#include <vector>

#include <lua/LuaBase.h>
#include <lua/LuaClass.h>
#include <lua/LuaConsole.h>


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
