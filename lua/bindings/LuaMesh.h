/*
 * Copyright 2015 Alexander Koch
 * File: LuaMesh.h
 * Description: Loading meshes in lua.
 *
 * mesh = Mesh()
 * mesh.loadCube(1.0)
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

#ifndef LUA_MESH_H
#define LUA_MESH_H

#include <memory>

#include "LuaMaterial.h"
#include <lua/LuaBase.h>
#include <lua/LuaClass.h>
#include <lua/LuaConsole.h>
#include <rendering/MeshRenderer.h>
#include <scene/MeshNode.h>

// Lua implementation of the MeshRenderer class
class LuaMesh : public LuaBase
{
public:
    LuaMesh(lua_State*);
    ~LuaMesh();

    int load(lua_State*);
    int loadCube(lua_State*);
    int loadPlane(lua_State*);

    int setPosition(lua_State*);
    int setScale(lua_State*);
    int setRotation(lua_State*);

    int getPosition(lua_State*);
    int getScale(lua_State*);
    int getRotation(lua_State*);

    int setCullFace(lua_State*);
    int setUseDepthMask(lua_State*);
    int setMaterial(lua_State*);

    int addChild(lua_State*);

    MeshRenderer* getModelObject();
    MeshNode* getSceneNode();
    static const std::string className;
    static const LuaClass<LuaMesh>::Function functions[];
    static const LuaClass<LuaMesh>::Property properties[];
private:
    std::shared_ptr<MeshRenderer> m_mesh;
    MeshNode m_node;
    int load_file(const std::string&);
};

#endif
