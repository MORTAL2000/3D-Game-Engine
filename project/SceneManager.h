/*
 * Copyright 2015 Alexander Koch
 * File: SceneManager.h
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
