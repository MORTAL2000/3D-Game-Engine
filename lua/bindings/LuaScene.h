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

/**
 *	Implementation of a Scene,
 * 	handles entities and provides a camera
 */
class LuaScene : public LuaBase
{
public:
	LuaScene(lua_State*);
	~LuaScene();

	int addEntity(lua_State*);
	int removeEntity(lua_State*);

	/**
	 *	Require: loads a material file
	 */
	int require(lua_State*);

	/**
	 *	loadLevel: EXPERIMENTAL: loads a 'scene'-file
	 */
	int loadLevel(lua_State*);

	/**
	 *	clear: Clears the scene / SceneNode
	 */
	int clear(lua_State*);

	/**
	 * 	Sets the scene gravity
	 */
	int setGravity(lua_State*);

	/**
	 *	Returns a camera reference
	 */
	int getDefaultCamera(lua_State*);

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
