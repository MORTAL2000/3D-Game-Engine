/*
 * Copyright 2015 Alexander Koch
 * File: LuaCamera.cpp
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

#include "LuaCamera.h"
#include "LuaMath.h"

LuaCamera::LuaCamera(lua_State* lua) : m_isObject(true)
{
	m_camera = new FilmCamera();
}

LuaCamera::LuaCamera(FilmCamera* camera) : m_isObject(false)
{
	m_camera = camera;
}

LuaCamera::~LuaCamera()
{
	if(m_isObject)
	{
		delete m_camera;
	}
}

int LuaCamera::setLookAt(lua_State* lua)
{
	m_camera->setTargetRaw(lua_read_vec3(lua, -1));
	return 0;
}

int LuaCamera::setPosition(lua_State* lua)
{
	m_camera->setPosition(lua_read_vec3(lua, -1));
	return 0;
}

int LuaCamera::setFlightMode(lua_State* lua)
{
	m_camera->setFlightMode(true);
	return 0;
}

int LuaCamera::setUpVector(lua_State* lua)
{
	m_camera->setUpVector(lua_read_vec3(lua, -1));
	return 0;
}

FilmCamera* LuaCamera::getModelObject()
{
	return m_camera;
}

const std::string LuaCamera::className = "Camera";
const LuaClass<LuaCamera>::Function LuaCamera::functions[] =
{
	{"lookAt", &LuaCamera::setLookAt},
	{"setPosition", &LuaCamera::setPosition},
	{"setFlightMode", &LuaCamera::setFlightMode},
	{"setUpVector", &LuaCamera::setUpVector},
	{0, 0}
};

const LuaClass<LuaCamera>::Property LuaCamera::properties[] =
{
	{0, 0}
};
