/*
 * Copyright 2015 Alexander Koch
 * File: LuaMaterial.h
 * Description: Lua implementation of the Material class.
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

#ifndef LUA_MATERIAL_H
#define LUA_MATERIAL_H

#include <lua/LuaBase.h>
#include <lua/LuaClass.h>
#include <lua/LuaConsole.h>
#include <util/MaterialLibrary.h>

class LuaMaterial : public LuaBase
{
public:
	LuaMaterial(lua_State*);
	~LuaMaterial();

	int set(lua_State*);
	int setValue(lua_State*);
	UserMaterial* getModelObject();

	static const std::string className;
	static const LuaClass<LuaMaterial>::Function functions[];
	static const LuaClass<LuaMaterial>::Property properties[];
private:
	UserMaterial* m_material;
};

#endif
