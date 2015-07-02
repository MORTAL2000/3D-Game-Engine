#include "LuaAPI.h"
#include <lua/bindings/LuaMath.h>
#include <core/Context.h>
#include <Engine.h>
#include <audio/WinMM.h>
#include <audio/WaveFile.h>

#define MOUSE_X "MOUSE_X"
#define MOUSE_Y "MOUSE_Y"

LuaAPI::LuaAPI() : m_lua(0)
{}

LuaAPI::~LuaAPI()
{
	finalize();
}

LuaAPI& LuaAPI::getInstance()
{
	static LuaAPI instance;
	return instance;
}

static int lua_quit(lua_State* lua)
{
	Core* core = Context::getInstance().getCore();
	if(core) core->close();
	return 0;
}

static int lua_centerCursor(lua_State* lua)
{
	Context::getInstance().centerCursor();
	return 0;
}

static int lua_cursor_visible(lua_State* lua)
{
	bool state = lua_toboolean(lua, -1);
	Context::getInstance().setCursorVisibility(state);
	return 0;
}

static int lua_seed(lua_State* lua)
{
	if(lua_isnumber(lua, -1))
	{
		seed(lua_tonumber(lua, -1));
	}
	return 0;
}

static int lua_simplex(lua_State* lua)
{
	double x = lua_tonumber(lua, -5);
	double y = lua_tonumber(lua, -4);
	double octaves = lua_tonumber(lua, -3);
	double persistence = lua_tonumber(lua, -2);
	double lacunarity = lua_tonumber(lua, -1);
	double value = simplex2(x, y, octaves, persistence, lacunarity);
	lua_pushnumber(lua, value);
	return 1;
}

static int lua_rotationMatrix(lua_State* lua)
{
	float angle = lua_tonumber(lua, -2);
	vec3 rotation = lua_read_vec3(lua, -1);
	mat4 result = glm::rotate(angle, rotation);
	LuaClass<LuaMatrix>::push(lua, new LuaMatrix(result));
	return 1;
}

static int lua_keydown(lua_State* lua)
{
	Engine* engine = dynamic_cast<Engine*>(Context::getInstance().getCore());
	if(engine)
	{
		if(lua_gettop(lua) == 1)
		{
			if(lua_type(lua, -1) == LUA_TSTRING)
			{
				std::string input = std::string(lua_tostring(lua, -1));
				if(input.size() != 1)
				{
					Console::log("isKeyDown(char key): key has to be one character");
					return 0;
				}

				char key = input[0];
				ActionListener* action = engine->getActionListener();
				bool state = (*action)[key];
				lua_pushboolean(lua, state);
				return 1;
			}
			else if(lua_type(lua, -1) == LUA_TNUMBER)
			{
				double number = lua_tonumber(lua, -1);
				if(number - floor(number) != 0)
				{
					Console::log("isKeyDown(char key): key has to be an integer");
					return 0;
				}
				char key = (int)number;
				ActionListener* action = engine->getActionListener();
				bool state = (*action)[key];
				lua_pushboolean(lua, state);
				return 1;
			}
			else
			{
				Console::log("isKeyDown(char key): key has to be either an integer or a character");
				return 0;
			}
		}
	}
	return 0;
}

static int lua_getAxis(lua_State* lua)
{
	Engine* engine = dynamic_cast<Engine*>(Context::getInstance().getCore());
	if(engine)
	{
		Mouse* mouse = engine->getMouse();
		if(lua_gettop(lua) == 1)
		{
			std::string axis = std::string(lua_tostring(lua, -1));
			if(axis == MOUSE_X)
			{
				lua_pushnumber(lua, mouse->position.x);
				return 1;
			}
			else if(axis == MOUSE_Y)
			{
				lua_pushnumber(lua, mouse->position.y);
				return 1;
			}
		}
	}
	return 0;
}

static int lua_play_sound(lua_State* lua)
{
	std::string file = std::string(lua_tostring(lua, -1));
	play_sound(file);
	return 0;
}

static int lua_takepic(lua_State* lua)
{
	std::string name = std::string(lua_tostring(lua, -1));
	Context::getInstance().takeScreenshot(name);
	return 0;
}

static int lua_check_sphere(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		float radius = lua_tonumber(lua, -2);
		vec3 pos = lua_read_vec3(lua, -1);

		Engine* engine = dynamic_cast<Engine*>(Context::getInstance().getCore());
		if(engine)
		{
			bool result = engine->getPhysicsEngine()->checkSphere(pos, pos, radius);
			lua_pushboolean(lua, result);
			return 1;
		}
	}
	return 0;
}

static int lua_draw_wire_cube(lua_State* lua)
{
	vec3 size = lua_read_vec3(lua, -2);
	vec3 center = lua_read_vec3(lua, -1);

	gl::begin();
	gl::drawWireCube(center, size);
	gl::end();
	return 0;
}

static int lua_draw_line(lua_State* lua)
{
	vec3 end = lua_read_vec3(lua, -2);
	vec3 start = lua_read_vec3(lua, -1);

	gl::begin();
	gl::drawLine(start, end);
	gl::end();
	return 0;
}

static int lua_set_color(lua_State* lua)
{
	vec4 color = lua_read_vec4(lua, -1);
	gl::setColor(color);
	return 0;
}

static int lua_draw_vector(lua_State* lua)
{
	vec3 end = lua_read_vec3(lua, -2);
	vec3 start = lua_read_vec3(lua, -1);

	gl::begin();
	gl::drawVector(start, end, 0.5, 0.1);
	gl::end();
	return 0;
}

void lua_class()
{
	std::stringstream code;
	code << "function class()\n";
	code << "	local cls = {}\n";
	code << "	cls.__index = cls\n";
	code <<	"return setmetatable(cls, {__call = function (c, ...)\n";
	code << "		local instance = setmetatable({}, cls)\n";
	code << "		if cls.__init then\n";
	code << "			cls.__init(instance, ...)\n";
	code << "		end\n";
	code <<	"	return instance\n";
	code << "	end})\n";
	code << "end\n";
	LuaAPI::getInstance().runCommand(code.str());
}

static int lua_include(lua_State* lua)
{
	std::string file = std::string(lua_tostring(lua, -1));
	std::string source;
	if(!FileReader::read(file, source))
	{
		return 0;
	}
	LuaAPI::getInstance().runCommand(source);
	return 0;
}

// register bindings, load lua
bool LuaAPI::initialize()
{
	if(isValid())
	{
		Console::log("Interpreter: Lua is already loaded");
		return false;
	}

	m_lua = luaL_newstate();
	luaL_openlibs(m_lua);
	lua_class();

	LuaClass<LuaMesh>::registerClass(m_lua, false);
	LuaClass<LuaMaterial>::registerClass(m_lua, false);
	LuaClass<LuaScene>::registerClass(m_lua, false);
	LuaClass<LuaMatrix>::registerClass(m_lua, true);
	LuaClass<LuaRigidBody>::registerClass(m_lua, false);
	LuaClass<LuaCamera>::registerClass(m_lua, false);

	add_enum_to_lua(m_lua, "Shape",
				"Sphere", LuaRigidBody::TYPE_SPHERE,
				"Box", LuaRigidBody::TYPE_BOX,
				"Cylinder", LuaRigidBody::TYPE_CYLINDER,
				"Mesh", LuaRigidBody::TYPE_MESH,
				0);

	registerFunction("dot", LuaMath::dot, 0);
	registerFunction("cross", LuaMath::cross, 0);
	registerFunction("length", LuaMath::length, 0);
	registerFunction("normalize", LuaMath::normalize, 0);
	registerFunction("sub", LuaMath::sub, 0);
	registerFunction("add", LuaMath::add, 0);
	registerFunction("clamp", LuaMath::clamp, "math");

	registerFunction("quit", lua_quit, 0);
	registerFunction("centerCursor", lua_centerCursor, 0);
	registerFunction("setCursorVisible", lua_cursor_visible, 0);

	registerFunction("seed", lua_seed, "noise");
	registerFunction("simplex", lua_simplex, "noise");
	registerFunction("rotation", lua_rotationMatrix, "matrix");

	registerFunction("isKeyDown", lua_keydown, 0);
	registerFunction("getAxis", lua_getAxis, 0);

	registerFunction("playSound", lua_play_sound, 0);
	registerFunction("screenshot", lua_takepic, 0);

	registerFunction("checkSphere", lua_check_sphere, "physics");

	registerFunction("setColor", lua_set_color, "debug");
	registerFunction("drawLine", lua_draw_line, "debug");
	registerFunction("drawVector", lua_draw_vector, "debug");
	registerFunction("drawWireCube", lua_draw_wire_cube, "debug");

	registerFunction("include", lua_include, 0);

	return true;
}

// releasing memory
void LuaAPI::finalize()
{
	if(m_lua != 0)
	{
		Console::log("Interpreter: Disposing lua components...");
		lua_clean(m_lua);
		lua_gc(m_lua, LUA_GCCOLLECT, 0);
		Console::log("Interpreter: Closing lua...");
		lua_close(m_lua);
		Console::log("Interpreter: Closed lua.");
		m_lua = 0;
	}
}

bool LuaAPI::registerFunction(const std::string& name, lua_CFunction function, const char* typespace = 0)
{
	if(!isValid())
	{
		Console::log("Interpreter: initialize before register a function");
		return false;
	}

	if(typespace)
	{
		lua_getglobal(m_lua, typespace);
		if(lua_isnil(m_lua, -1))
		{
			lua_newtable(m_lua);
			lua_pushvalue(m_lua, -1);
			lua_setglobal(m_lua, typespace);
		}
		lua_pushcfunction(m_lua, function);
		lua_setfield(m_lua, -2, name.c_str());
		lua_pop(m_lua, 1);
	}
	else
	{
		lua_register(m_lua, name.c_str(), function);
	}

	return true;
}

lua_State* LuaAPI::getLuaState()
{
	return m_lua;
}

bool LuaAPI::load(const std::string& filename, LuaScript& script)
{
	if(!isValid() || filename.empty())
	{
		Console::log("Interpreter: Could not load script");
		script = LuaScript(0, filename);
		return false;
	}
	script = LuaScript(m_lua, filename);
	return true;
}

bool LuaAPI::runCommand(const std::string& command)
{
	if(!isValid())
	{
		Console::log("Interpreter: Could not run command, context is invalid");
		return false;
	}

	if(luaL_dostring(m_lua, command.c_str()))
	{
		Console::log("Interpreter: Invalid command");
		return false;
	}
	lua_clean(m_lua);
	return true;
}

bool LuaAPI::isValid()
{
	return (m_lua != 0);
}
