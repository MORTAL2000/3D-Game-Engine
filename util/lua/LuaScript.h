#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <string>

#include "LuaBase.h"
#include "../../core/Tokenizer.h"
#include "../../core/FileReader.h"

class LuaScript
{
public:
    LuaScript();
    LuaScript(lua_State*, const std::string&);
    LuaScript(const std::string&);

    bool reload();
    bool isValid();

    void run();
    bool runFunction(const std::string&);
    bool hasFunction(const std::string&);
    bool hasVariable(const std::string&);

    template<typename T>
    T get(const std::string& name)
    {
        if(!m_lua)
        {
            return lua_getdefault<T>();
        }

        T result;
        if(lua_gettostack(name))
        {
            result = lua_get<T>(name);
        }
        else
        {
            result = lua_getdefault<T>();
        }

        lua_clean(m_lua);
        return result;
    }

    std::vector<std::string> getTableKeys(const std::string&);
    std::vector<float> getNumberVector(const std::string&);
    int getType(const std::string&);
    std::string getTypeString(int);

    const std::string getFilename();
    const std::string getName();

    void close();
private:
    lua_State* m_lua;
    std::string m_filename;

    template<typename T>
    T lua_getdefault()
    {
        return 0;
    }

    template<typename T>
    T lua_get(const std::string& name)
    {
        return 0;
    }

    bool lua_gettostack(const std::string&);
};

template <>
inline bool LuaScript::lua_get<bool>(const std::string& name)
{
    return (bool)lua_toboolean(m_lua, -1);
}

template <>
inline float LuaScript::lua_get<float>(const std::string& name)
{
    if(!lua_isnumber(m_lua, -1))
    {
        Console::log("%s is not a number", name.c_str());
    }
    return (float)lua_tonumber(m_lua, -1);
}

template <>
inline int LuaScript::lua_get<int>(const std::string& name)
{
    if(!lua_isnumber(m_lua, -1))
    {
        Console::log("%s is not a number", name.c_str());
    }
    return (int)lua_tonumber(m_lua, -1);
}

template <>
inline std::string LuaScript::lua_get<std::string>(const std::string& name)
{
    std::string s = "null";
    if(lua_isstring(m_lua, -1))
    {
        s = std::string(lua_tostring(m_lua, -1));
    }
    else
    {
        Console::log("%s is not a string", name.c_str());
    }
    return s;
}

template<>
inline std::string LuaScript::lua_getdefault<std::string>()
{
    return "null";
}

#endif
