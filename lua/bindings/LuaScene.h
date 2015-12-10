/*
 * Copyright 2015 Alexander Koch
 * File: LuaScene.h
 * Description: Lua's scene class.
 * Used for standard scripts:
 *
 * scene = Scene()
 * scene.addEntity()
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

#ifndef LUA_SCENE_H
#define LUA_SCENE_H

#include <string>

#include "LuaCamera.h"
#include "LuaMaterial.h"
#include <lua/LuaBase.h>
#include <lua/LuaClass.h>
#include <lua/LuaConsole.h>

#include <scene/SceneNode.h>
#include <project/SceneManager.h>
#include <physics/BulletPhysicsEngine.h>
#include <physics/BulletBody.h>
#include <util/FilmCamera.h>

#include <rendering/ShadowMap.h>

// Implementation of a Scene,
// handles entities and provides a camera
class LuaScene : public LuaBase
{
public:
	LuaScene(lua_State*);
	~LuaScene();

	int addEntity(lua_State*);
	int removeEntity(lua_State*);

	int require(lua_State*);
	int loadLevel(lua_State*);
	int clear(lua_State*);
	int setGravity(lua_State*);
	int getDefaultCamera(lua_State*);

	int loadMeshes(lua_State*);

	// EXPERIMENTAL:
	// Directional shadow mapping see 'game'-demo
	int enableShadows(lua_State*);
	int renderShadows(lua_State*);
	int passShadowInfo(lua_State*);

	static const std::string className;
	static const LuaClass<LuaScene>::Function functions[];
	static const LuaClass<LuaScene>::Property properties[];
private:
	SceneNode* m_scene;
	ShadowMap m_shadow_map;
};

#endif
