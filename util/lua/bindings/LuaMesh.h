#ifndef LUA_MESH_H
#define LUA_MESH_H

#include <memory>

#include "LuaMaterial.h"
#include "../LuaBase.h"
#include "../LuaClass.h"
#include "../LuaConsole.h"
#include "../../../rendering/MeshRenderer.h"
#include "../../../scene/MeshNode.h"

/**
 *  Lua implementation of the MeshRenderer class
 */
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
