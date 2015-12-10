/*
 * Copyright 2015 Alexander Koch
 * File: LuaAPI.h
 * Description: Lua interpreter helper functions.
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

#ifndef LUA_API_H
#define LUA_API_H

#include <cstring>
#include <sstream>
#include <vector>

#include "LuaBase.h"
#include "LuaScript.h"
#include "LuaClass.h"
#include "LuaEnums.h"

#include <lua/bindings/LuaMesh.h>
#include <lua/bindings/LuaMaterial.h>
#include <lua/bindings/LuaScene.h>
#include <lua/bindings/LuaMatrix.h>
#include <lua/bindings/LuaRigidBody.h>
#include <lua/bindings/LuaCamera.h>

#include <rendering/GL.h>
#include <util/Noise.h>

#include <network/Server.h>
#include <audio/AudioEngine.h>

typedef int (*lua_CFunction) (lua_State *lua);

class LuaAPI
{
public:
    static LuaAPI& getInstance();

    bool initialize();
    void finalize();

    bool registerFunction(const std::string&, lua_CFunction, const char*);
    lua_State* getLuaState();

    bool load(const std::string&, LuaScript&);
    bool runCommand(const std::string&);
private:
    lua_State* m_lua;

    LuaAPI();
    ~LuaAPI();
    bool isValid();
};

#endif
