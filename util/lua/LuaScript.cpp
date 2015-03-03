#include "LuaScript.h"

LuaScript::LuaScript() :
	m_lua(0), m_filename("")
{}

LuaScript::LuaScript(lua_State* lua, const std::string& filename) :
	m_lua(lua), m_filename(filename)
{
	reload();
}

LuaScript::LuaScript(const std::string& filename) : m_filename(filename)
{
	m_lua = luaL_newstate();
	luaL_openlibs(m_lua);
	reload();
}

bool LuaScript::reload()
{
	if(!isValid()) return false;
	std::string source;
	if(!FileReader::read(m_filename, &source))
	{
		Console::log("Lua :: error loading file(%s)", m_filename.c_str());
		return false;
	}
	if(luaL_dostring(m_lua, source.c_str()))
	{
		Console::log("Lua :: error loading file(%s)\ndescription: %s", m_filename.c_str(), lua_tostring(m_lua, -1));
	}
	lua_clean(m_lua);
	return true;
}

bool LuaScript::isValid()
{
	if(!m_lua)
	{
		Console::log("Lua :: Script state is invalid, load script from LuaAPI class");
		return false;
	}
	if(m_filename.empty())
	{
		Console::log("Lua :: Invalid filename");
		return false;
	}
	return true;
}

void LuaScript::run()
{
	if(isValid()) luaL_dofile(m_lua, m_filename.c_str());
}

bool LuaScript::runFunction(const std::string& function)
{
	const char* cfunction = function.c_str();
	if(!isValid() || function.empty())
	{
		Console::log("Lua :: Could not run function '%s'", cfunction);
		return false;
	}

	if(!hasFunction(function))
	{
		Console::log("Lua :: '%s' is not a function", cfunction);
		return false;
	}

	if(lua_pcall(m_lua, 0, 0, 0) != 0) {
		Console::log("Lua :: error running function(%s)\ndescription: %s", cfunction, lua_tostring(m_lua, -1));
		return false;
	}

	lua_clean(m_lua);
	return true;
}

bool LuaScript::hasFunction(const std::string& functionName)
{
	if(functionName.size() == 0 || functionName == "") return false;
	lua_getglobal(m_lua, functionName.c_str());
	return lua_isfunction(m_lua, -1);
}

std::vector<std::string> LuaScript::getTableKeys(const std::string& name)
{
	std::vector<std::string> strings;
	lua_gettostack(name);
	lua_pushnil(m_lua);
	while(lua_next(m_lua, -2))
	{
		strings.push_back(std::string(lua_tostring(m_lua, -2)));
		lua_pop(m_lua, 1);
	}
	lua_clean(m_lua);
	return strings;
}

std::vector<float> LuaScript::getNumberVector(const std::string& name)
{
	std::vector<float> values;
	lua_gettostack(name);
	lua_pushnil(m_lua);
	while(lua_next(m_lua, -2))
	{
		values.push_back((float)lua_tonumber(m_lua, -1));
		lua_pop(m_lua, 1);
	}
	lua_clean(m_lua);
	return values;
}

int LuaScript::getType(const std::string& name)
{
	lua_gettostack(name);
	return lua_type(m_lua, -1);
}

std::string LuaScript::getTypeString(int type)
{
	return std::string(lua_typename(m_lua, type));
}

const std::string LuaScript::getFilename()
{
	return m_filename;
}

const std::string LuaScript::getName()
{
	return Tokenizer::removePath(m_filename);
}

void LuaScript::close()
{
	if(m_lua)
	{
		lua_close(m_lua);
	}
}

bool LuaScript::lua_gettostack(const std::string& name)
{
	int level = 0;
	std::string var = "";
	for(auto i = 0; i < name.size(); i++)
	{
		if(name.at(i) == '.')
		{
			if(level == 0)
			{
				lua_getglobal(m_lua, var.c_str());
			}
			else
			{
				lua_getfield(m_lua, -1, var.c_str());
			}

			if(lua_isnil(m_lua, -1))
			{
				Console::log("Variable %s not defined", name.c_str());
				return false;
			}
			else
			{
				var = "";
				level++;
			}
		}
		else
		{
			var += name.at(i);
		}
	}

	if(level == 0)
	{
		lua_getglobal(m_lua, var.c_str());
	}
	else
	{
		lua_getfield(m_lua, -1, var.c_str());
	}

	if(lua_isnil(m_lua, -1))
	{
		Console::log("Variable %s not defined", name.c_str());
		return false;
	}

	return true;
}
