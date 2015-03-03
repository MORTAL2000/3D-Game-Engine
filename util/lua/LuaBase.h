#ifndef LUA_BASE_H
#define LUA_BASE_H

#include <string>

#include "../../core/Console.h"
#include "../../core/Commons.h"

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

void lua_clean(lua_State*);

class LuaBase
{
public:
    virtual void add(lua_State*, LuaBase*) {}
    virtual void sub(lua_State*, LuaBase*) {}

    virtual void mul(lua_State*, LuaBase*) {}
    virtual void div(lua_State*, LuaBase*) {}

    virtual bool eq(LuaBase*) { return false; }
    virtual std::string tostring() { return ""; }
};

#endif
