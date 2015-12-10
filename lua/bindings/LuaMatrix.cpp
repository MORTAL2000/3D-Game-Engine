/*
 * Copyright 2015 Alexander Koch
 * File: LuaMatrix.cpp
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

#include "LuaMatrix.h"

LuaMatrix::LuaMatrix(lua_State* lua)
{
	int args = lua_gettop(lua);
	if(args == 1)
	{
		if(lua_isnumber(lua, -1))
		{
			double value = lua_tonumber(lua, -1);
			m_matrix = mat4(value);
		}
	}
}

LuaMatrix::LuaMatrix(const mat4& matrix)
{
	m_matrix = matrix;
}

LuaMatrix::~LuaMatrix()
{

}

/**
 *	Returns element at position
 *  C++ Notation:
 *	double get(int column, int row);
 */
int LuaMatrix::get(lua_State* lua)
{
	if(lua_isnumber(lua, -1))
	{
		int column = lua_tonumber(lua, -2);
		int row = lua_tonumber(lua, -1);
		lua_pushnumber(lua, m_matrix[column][row]);
		return 1;
	}
	else
	{
		LuaConsole::error(lua, "%s:get(int column, int row): row and column has to be integers", className.c_str());
	}
	return 0;
}

/**
 *	Sets element at position
 *  C++ Notation:
 *	double set(int column, int row, double value);
 */
int LuaMatrix::set(lua_State* lua)
{
	//glm matrix is column major [column][row], so invert
	if(lua_isnumber(lua, -1) && lua_isnumber(lua, -2) && lua_isnumber(lua, -3))
	{
		int column = lua_tonumber(lua, -3);
		int row = lua_tonumber(lua, -2);
		double value = lua_tonumber(lua, -1);
		m_matrix[column][row] = value;
	}
	else
	{
		LuaConsole::error(lua, "Usage: %s:set(int column, int row, double value)", className.c_str());
	}
	return 0;
}

void LuaMatrix::mul(lua_State* lua, LuaBase* base)
{
	LuaMatrix* matrix = dynamic_cast<LuaMatrix*>(base);
	if(matrix)
	{
		LuaClass<LuaMatrix>::push(lua, new LuaMatrix(m_matrix * matrix->getModelObject()));
	}
}

std::string LuaMatrix::tostring()
{
	return "Not implemented";
}

mat4 LuaMatrix::getModelObject()
{
	return m_matrix;
}

const std::string LuaMatrix::className = "mat4";
const LuaClass<LuaMatrix>::Function LuaMatrix::functions[] =
{
	{"get", &LuaMatrix::get},
	{"set", &LuaMatrix::set},
	{0, 0}
};

const LuaClass<LuaMatrix>::Property LuaMatrix::properties[] =
{
	{0, 0}
};
