/*
 * Copyright 2015 Alexander Koch
 * File: LuaMatrix.h
 * Description: Lua implementation of a 4x4 Matrix.
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

#ifndef LUA_MATRIX_H
#define LUA_MATRIX_H

#include <lua/LuaBase.h>
#include <lua/LuaClass.h>
#include <lua/LuaConsole.h>

class LuaMatrix : public LuaBase
{
public:
	LuaMatrix(lua_State*);
	LuaMatrix(const mat4&);
	~LuaMatrix();

	int get(lua_State*);
	int set(lua_State*);

	virtual void mul(lua_State*, LuaBase*);
	virtual std::string tostring();

	mat4 getModelObject();
	static const std::string className;
	static const LuaClass<LuaMatrix>::Function functions[];
	static const LuaClass<LuaMatrix>::Property properties[];
private:
	mat4 m_matrix;
};

#endif
