#ifndef LUA_API_H
#define LUA_API_H

#include <cstring>
#include <sstream>
#include <vector>

#include "LuaBase.h"
#include "LuaScript.h"
#include "LuaClass.h"
#include "LuaEnums.h"

#include "bindings/LuaMesh.h"
#include "bindings/LuaMaterial.h"
#include "bindings/LuaScene.h"
#include "bindings/LuaMatrix.h"
#include "bindings/LuaRigidBody.h"
#include "bindings/LuaCamera.h"

#include "../../rendering/GL.h"
#include "../Noise.h"

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
