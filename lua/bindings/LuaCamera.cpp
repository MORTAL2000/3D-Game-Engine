#include "LuaCamera.h"

#include "LuaMath.h"

LuaCamera::LuaCamera(lua_State* lua) : m_isObject(true)
{
	m_camera = new FilmCamera();
}

LuaCamera::LuaCamera(FilmCamera* camera) : m_isObject(false)
{
	m_camera = camera;
}

LuaCamera::~LuaCamera()
{
	if(m_isObject)
	{
		delete m_camera;
	}
}

int LuaCamera::setLookAt(lua_State* lua)
{
	m_camera->setTargetRaw(lua_read_vec3(lua, -1));
	return 0;
}

int LuaCamera::setPosition(lua_State* lua)
{
	m_camera->setPosition(lua_read_vec3(lua, -1));
	return 0;
}

int LuaCamera::setFlightMode(lua_State* lua)
{
	m_camera->setFlightMode(true);
	return 0;
}

int LuaCamera::setUpVector(lua_State* lua)
{
	m_camera->setUpVector(lua_read_vec3(lua, -1));
	return 0;
}

FilmCamera* LuaCamera::getModelObject()
{
	return m_camera;
}

const std::string LuaCamera::className = "Camera";
const LuaClass<LuaCamera>::Function LuaCamera::functions[] =
{
	{"lookAt", &LuaCamera::setLookAt},
	{"setPosition", &LuaCamera::setPosition},
	{"setFlightMode", &LuaCamera::setFlightMode},
	{"setUpVector", &LuaCamera::setUpVector},
	{0, 0}
};

const LuaClass<LuaCamera>::Property LuaCamera::properties[] =
{
	{0, 0}
};
