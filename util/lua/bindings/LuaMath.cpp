#include "LuaMath.h"

// main functions
void lua_push_vector(lua_State* lua, const float* array, int size)
{
	lua_newtable(lua);
	for(int i = 0; i < size; i++)
	{
		lua_pushnumber(lua, i+1);
		lua_pushnumber(lua, array[i]);
		lua_settable(lua, -3);
	}
}

std::vector<float> lua_read_vector(lua_State* lua, int index)
{
	std::vector<float> content;
	if(lua_istable(lua, index))
	{
		lua_pushnil(lua);
		while(lua_next(lua, -2))
		{
			float value = (float)lua_tonumber(lua, -1);
			content.push_back(value);
			lua_pop(lua, 1);
		}
		lua_pop(lua, 1);
	}

	return content;
}

// wrappers

void lua_push_vec3(lua_State* lua, const vec3& table)
{
	lua_push_vector(lua, glm::value_ptr(table), 3);
}

void lua_push_vec4(lua_State* lua, const vec4& table)
{
	lua_push_vector(lua, glm::value_ptr(table), 4);
}

vec3 lua_read_vec3(lua_State* lua, int index)
{
	vec3 final;
	std::vector<float> vec = lua_read_vector(lua, index);
	if(vec.size() == 3)
	{
		for(auto i = 0; i < vec.size(); i++)
		{
			final[i] = vec[i];
		}
	}
	return final;
}

vec4 lua_read_vec4(lua_State* lua, int index)
{
	vec4 final;
	std::vector<float> vec = lua_read_vector(lua, index);
	if(vec.size() == 4)
	{
		for(auto i = 0; i < vec.size(); i++)
		{
			final[i] = vec[i];
		}
	}
	return final;
}

namespace LuaMath
{

// operators

int sub(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		std::vector<float> v1 = lua_read_vector(lua, -2);
		std::vector<float> v0 = lua_read_vector(lua, -1);

		if(v0.size() == v1.size())
		{
			vec3 final;
			for(auto i = 0; i < v0.size(); i++)
			{
				final[i] = v0[i] - v1[i];
			}
			lua_push_vec3(lua, final);
			return 1;
		}
	}
	return 0;
}

int add(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		std::vector<float> v1 = lua_read_vector(lua, -2);
		std::vector<float> v0 = lua_read_vector(lua, -1);

		if(v0.size() == v1.size())
		{
			vec3 final;
			for(auto i = 0; i < v0.size(); i++)
			{
				final[i] = v0[i] + v1[i];
			}
			lua_push_vec3(lua, final);
			return 1;
		}
	}
	return 0;
}

int dot(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		std::vector<float> v1 = lua_read_vector(lua, -2);
		std::vector<float> v0 = lua_read_vector(lua, -1);

		if(v0.size() == v1.size())
		{
			float value = 0;
			for(auto i = 0; i < v0.size(); i++)
			{
				value += v0[i] * v1[i];
			}
			lua_pushnumber(lua, value);
			return 1;
		}
	}
	return 0;
}

int cross(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		std::vector<float> v1 = lua_read_vector(lua, -2);
		std::vector<float> v0 = lua_read_vector(lua, -1);

		if(v0.size() == 3 && v1.size() == 3)
		{
			vec3 vI, vJ;
			for(int i = 0; i < 3; i++)
			{
				vI[i] = v0[i];
				vJ[i] = v1[i];
			}
			vec3 crossV = glm::cross(vI, vJ);
			lua_push_vec3(lua, crossV);
			return 1;
		}
	}
	return 0;
}

int length(lua_State* lua)
{
	if(lua_gettop(lua) == 1)
	{
		std::vector<float> v0 = lua_read_vector(lua, -1);

		float value = 0;
		for(auto i = 0; i < v0.size(); i++)
		{
			value += v0[i] * v0[i];
		}
		value = sqrt(value);
		lua_pushnumber(lua, value);
		return 1;
	}
	return 0;
}

int normalize(lua_State* lua)
{
	if(lua_gettop(lua) == 1)
	{
		std::vector<float> v0 = lua_read_vector(lua, -1);

		if(v0.size() == 3)
		{
			vec3 v1;
			for(auto i = 0; i < 3; i++)
			{
				v1[i] = v0[i];
			}
			lua_push_vec3(lua, glm::normalize(v1));
			return 1;
		}

		if(v0.size() == 4)
		{
			vec4 v1;
			for(auto i = 0; i < 4; i++)
			{
				v1[i] = v0[i];
			}
			lua_push_vec4(lua, glm::normalize(v1));
			return 1;
		}
	}
	return 0;
}

// normal

int clamp(lua_State* lua)
{
	if(lua_gettop(lua) == 3)
	{
		float val = lua_tonumber(lua, -3);
		float min = lua_tonumber(lua, -2);
		float max = lua_tonumber(lua, -1);
		float final = std::max(min, std::min(max, val));
		lua_pushnumber(lua, final);
		return 1;
	}
	return 0;
}

}
