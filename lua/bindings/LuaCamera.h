/*
 * Copyright 2015 Alexander Koch
 * File: LuaCamera.h
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

#ifndef LUA_CAMERA_H
#define LUA_CAMERA_H

#include <lua/LuaBase.h>
#include <lua/LuaClass.h>
#include <lua/LuaConsole.h>
#include <lua/bindings/LuaMesh.h>
#include <util/FilmCamera.h>

class LuaCamera : public LuaBase
{
public:
	LuaCamera(lua_State*);
	LuaCamera(FilmCamera*);
	~LuaCamera();

	int setLookAt(lua_State*);
	int setPosition(lua_State*);
	int setFlightMode(lua_State*);
	int setUpVector(lua_State*);

	FilmCamera* getModelObject();

	static const std::string className;
	static const LuaClass<LuaCamera>::Function functions[];
	static const LuaClass<LuaCamera>::Property properties[];
private:
	FilmCamera* m_camera;
	bool m_isObject;
};

#endif
