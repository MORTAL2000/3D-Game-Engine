#include "LuaMaterial.h"

/**
 *	Lua material constructor
 *	C++ Notation:
 *	Material()
 * 	Material(string);
 */
LuaMaterial::LuaMaterial(lua_State* lua) : m_material(0)
{
	if(lua_gettop(lua) == 1)
	{
		if(!lua_isstring(lua, -1))
		{
			LuaConsole::error(lua, "Material(string id): id is not a string");
		}
		else
		{
			UserMaterial* temp = MaterialLibrary::getInstance().get(lua_tostring(lua, -1));
			if(!temp)
			{
				LuaConsole::error(lua, "Material:set(string id): id is not found (JSON Material)");
			}
			else
			{
				m_material = temp;
			}
		}
	}
}

LuaMaterial::~LuaMaterial()
{}

/**
 *	Sets the material pointer to another material
 *	C++ Notation:
 *	void set(string);
 */
int LuaMaterial::set(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		if(!lua_isstring(lua, -1))
		{
			LuaConsole::error(lua, "Material:set(string id): id is not a string");
			return 0;
		}

		UserMaterial* temp = MaterialLibrary::getInstance().get(lua_tostring(lua, -1));
		if(!temp)
		{
			LuaConsole::error(lua, "Material:set(string id): id is not found (JSON Material)");
			return 0;
		}
		m_material = temp;
	}
	else
	{
		LuaConsole::error(lua, "Usage: Material:set(string id)");
	}
	return 0;
}

/**
 *	Sets a uniform value of a JSON UserMaterial
 *  C++ Notation:
 * 	void setValue(string, int);
 * 	void setValue(string, double);
 * 	void setValue(string, double, double, double);
 *	void setValue(string, double, double, double, double);
 * 	void setValue(string, string);
 */
int LuaMaterial::setValue(lua_State* lua)
{
	int args = lua_gettop(lua);
	if(m_material)
	{
		if(args == 3)
		{
			//key + one value
			if(!lua_isstring(lua, -2)) return 0;
			std::string key = std::string(lua_tostring(lua, -2));
			GenericType value;
			switch(lua_type(lua, -1))
			{
				case LUA_TSTRING:
				{
					value = GenericType(string(lua_tostring(lua, -1)));
					break;
				}
				case LUA_TNUMBER:
				{
					double temp = lua_tonumber(lua, -1);
					if(temp - floor(temp) == 0)
					{
						value = GenericType((int)temp);
					}
					else
					{
						value = GenericType(temp);
					}
					break;
				}
				case LUA_TBOOLEAN:
				{
					value = GenericType(lua_toboolean(lua, -1));
					break;
				}
				default:
				{
					return 0;
				}
			}

			m_material->updateValue(key, value);
		}
		else if(args == 5)
		{
			//key plus vector3
			if(!lua_isstring(lua, -4)) return 0;

			std::string key = string(lua_tostring(lua, -4));
			std::vector<double> vector;
			for(uint8_t j = 3; j > 0; j--)
			vector.push_back(lua_tonumber(lua, -j));
			m_material->updateValue(key, GenericType(vector));
		}
		else if(args == 6)
		{
			//key plus vector4
			if(!lua_isstring(lua, -5)) return 0;

			std::string key = string(lua_tostring(lua, -5));
			std::vector<double> vector;
			for(uint8_t j = 4; j > 0; j--)
			vector.push_back(lua_tonumber(lua, -j));

			m_material->updateValue(key, GenericType(vector));
		}
	}
	else
	{
		LuaConsole::error(lua, "Material is invalid");
	}
	return 0;
}

/**
 *	Returns pointer of the model object
 */
UserMaterial* LuaMaterial::getModelObject()
{
	return m_material;
}

const std::string LuaMaterial::className = "Material";
const LuaClass<LuaMaterial>::Function LuaMaterial::functions[] =
{
	{"set", &LuaMaterial::set},
	{"setValue", &LuaMaterial::setValue},
	{0, 0}
};

const LuaClass<LuaMaterial>::Property LuaMaterial::properties[] =
{
	{0, 0}
};
