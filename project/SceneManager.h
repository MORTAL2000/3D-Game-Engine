#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <string>
#include <vector>

#include <core/Console.h>
#include <core/StringUtils.h>
#include <core/Tokenizer.h>
#include <lua/LuaAPI.h>
#include <lua/LuaScript.h>
#include <util/MaterialLibrary.h>
#include <scene/SceneNode.h>
#include <scene/MeshNode.h>
#include <gui/RenderableNode.h>
#include <rendering/materials/UserMaterial.h>

// Usage
//SceneManager::getInstance().load("levels/physics.lvl");

class SceneManager
{
public:
    static SceneManager getInstance();
    ~SceneManager();

    void load(const std::string&);
    bool exportScene(const std::string&, SceneNode*);
private:
    SceneManager();
};

#endif
