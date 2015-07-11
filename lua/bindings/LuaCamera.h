#ifndef LUA_CAMERA_H
#define LUA_CAMERA_H

#include <lua/LuaBase.h>
#include <lua/LuaClass.h>
#include <lua/LuaConsole.h>
#include <lua/bindings/LuaMesh.h>
#include <util/FilmCamera.h>

class LuaCamera : public LuaBase
{
public:
	LuaCamera(lua_State*);
	LuaCamera(FilmCamera*);
	~LuaCamera();

	int setLookAt(lua_State*);
	int setPosition(lua_State*);
	int setFlightMode(lua_State*);
	int setUpVector(lua_State*);

	FilmCamera* getModelObject();

	static const std::string className;
	static const LuaClass<LuaCamera>::Function functions[];
	static const LuaClass<LuaCamera>::Property properties[];
private:
	FilmCamera* m_camera;
	bool m_isObject;
};

#endif
