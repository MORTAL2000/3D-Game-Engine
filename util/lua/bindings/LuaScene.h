#ifndef LUA_SCENE_H
#define LUA_SCENE_H

#include <string>

#include "LuaCamera.h"
#include "LuaMaterial.h"
#include "../LuaBase.h"
#include "../LuaClass.h"
#include "../LuaConsole.h"

#include "../../../scene/SceneNode.h"
#include "../../../project/SceneManager.h"
#include "../../../physics/BulletPhysicsEngine.h"
#include "../../../physics/BulletBody.h"
#include "../../FilmCamera.h"

#include "../../../rendering/ShadowMap.h"

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

	int require(lua_State*);
	int loadLevel(lua_State*);
	int clear(lua_State*);
	int setGravity(lua_State*);

	int getDefaultCamera(lua_State*);

	//dev
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
