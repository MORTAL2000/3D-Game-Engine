/*
 * Copyright 2015 Alexander Koch
 * File: LuaBase.h
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

#ifndef LUA_BASE_H
#define LUA_BASE_H

#include <string>

#include <core/Console.h>
#include <core/Commons.h>

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
