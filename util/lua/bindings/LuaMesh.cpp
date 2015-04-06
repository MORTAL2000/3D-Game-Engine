#include "LuaMesh.h"

#include "LuaMath.h"

/**
 *  Mesh constructor
 *  C++ Notation:
 *  Mesh();
 *  Mesh(string);
 */
LuaMesh::LuaMesh(lua_State* lua) : m_mesh(new MeshRenderer()), m_node(m_mesh.get())
{
	if(lua_gettop(lua) == 1)
	{
		if(!lua_isstring(lua, -1))
		{
			LuaConsole::error(lua, "%s(string path): path is not a string", className.c_str());
		}
		else load_file(std::string(lua_tostring(lua, -1)));
	}
	m_mesh->setMaterial(MaterialLibrary::getInstance().getDefault());
}

LuaMesh::~LuaMesh()
{}

/**
 *  Load mesh from the given path
 *  C++ Notation:
 *  void load(std::string);
 */
int LuaMesh::load(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		if(lua_type(lua, -1) != LUA_TSTRING)
		{
			LuaConsole::error(lua, "%s:load(string path): path is not a string", className.c_str());
			return 0;
		}
		load_file(std::string(lua_tostring(lua, -1)));
	}
	else
	{
		LuaConsole::error(lua, "%s:load(string path): function requires one parameter", className.c_str());
	}
	return 0;
}

/**
 *  Loads a cube
 *  C++ Notation:
 *  void loadCube(double scale);
 *	void loadCube(vec3 position, vec3 scale)
 */
int LuaMesh::loadCube(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		if(!lua_isnumber(lua, -1))
		{
			LuaConsole::error(lua, "%s:loadCube(double scale): scale is not a number", className.c_str());
			return 0;
		}

		m_mesh->loadCube(lua_tonumber(lua, -1));
	}
	else if(lua_gettop(lua) == 3)
	{
		vec3 scale = lua_read_vec3(lua, -2);
		vec3 position = lua_read_vec3(lua, -1);
		m_mesh->loadCube(position, scale);
	}
	else
	{
		LuaConsole::error(lua, "%s:loadCube(double scale): function requires one parameter", className.c_str());
	}
	return 0;
}

/**
 *  Loads a plane
 *  C++ Notation:
 *  void loadPlane(double scale);
 */
int LuaMesh::loadPlane(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		if(!lua_isnumber(lua, -1))
		{
			LuaConsole::error(lua, "%s:loadPlane(double scale): scale is not a number", className.c_str());
			return 0;
		}

		m_mesh->loadPlane(lua_tonumber(lua, -1));
	}
	else
	{
		LuaConsole::error(lua, "%s:loadPlane(double scale): function requires one parameter", className.c_str());
	}
	return 0;
}

/**
 *  Sets the position of the current mesh
 *  C++ Notation:
 *  void setPosition(Vector3 position);
 */
int LuaMesh::setPosition(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		m_mesh->setPosition(lua_read_vec3(lua, -1));
	}
	else
	{
		LuaConsole::error(lua, "%s:setPosition(double x, double y, double z): function requires three parameters", className.c_str());
	}
	return 0;
}

/**
 *  Sets the scale of the current mesh
 *  C++ Notation:
 *  void setScale(Vector3 scale);
 */
int LuaMesh::setScale(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		m_mesh->setScale(lua_read_vec3(lua, -1));
	}
	else
	{
		LuaConsole::error(lua, "Usage: %s:setScale(double x, double y, double z) || %s:setScale(double scale)", className.c_str(), className.c_str());
	}
	return 0;
}

/**
 *  Sets the rotation of the current mesh
 *  C++ Notation:
 *  void setRotation(Vector3 rotation);
 */
int LuaMesh::setRotation(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		m_mesh->setRotation(lua_read_vec3(lua, -1));
	}
	else
	{
		LuaConsole::error(lua, "%s:setRotation(double x, double y, double z): function requires three parameters", className.c_str());
	}
	return 0;
}

/**
 *	Returns the current position
 *	C++ Notation:
 *	Vector3 getPosition();
 */
int LuaMesh::getPosition(lua_State* lua)
{
	if(lua_gettop(lua) == 1)
	{
		lua_push_vec3(lua, m_mesh->getPosition());
		return 1;
	}
	else
	{
		LuaConsole::error(lua, "%s:getPosition() requires no arguments", className.c_str());
	}
	return 0;
}

/**
 *	Returns the current scale
 *  C++ Notation:
 *	Vector3 getScale();
 */
int LuaMesh::getScale(lua_State* lua)
{
	if(lua_gettop(lua) == 1)
	{
		lua_push_vec3(lua, m_mesh->getScale());
		return 1;
	}
	else
	{
		LuaConsole::error(lua, "%s:getScale() requires no arguments", className.c_str());
	}

	return 0;
}

/**
 *	Returns the current rotation
 * 	C++ Notation:
 * 	Quaterion getRotation();
 */
int LuaMesh::getRotation(lua_State* lua)
{
	if(lua_gettop(lua) == 1)
	{
		quat rotation = m_mesh->getRotation();
		vec4 converted = vec4(rotation[0], rotation[1], rotation[2], rotation[3]);
		lua_push_vec4(lua, converted);
		return 1;
	}
	else
	{
		LuaConsole::error(lua, "%s:getRotation() requires no arguments", className.c_str());
	}

	return 0;
}

/**
 *	Sets wich faces to cull
 *  C++ Notation:
 *  void setCullFace(string state);
 */
int LuaMesh::setCullFace(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		if(!lua_isstring(lua, -1))
		{
			LuaConsole::error(lua, "%s:setCullFace(string face): face has to be a string", className.c_str());
			return 0;
		}

		std::string face = std::string(lua_tostring(lua, -1));
		if(face == "front")
		{
			m_mesh->setCullFace(GL_FRONT);
		}
		else if(face == "back")
		{
			m_mesh->setCullFace(GL_BACK);
		}
		else if(face == "none")
		{
			m_mesh->setCullFace(GL_NONE);
		}
		else
		{
			LuaConsole::error(lua, "%s:setCullFace(string face): face has to be 'front' or 'back'", className.c_str());
		}
	}
	return 0;
}

/**
 *	Enables or disables rendering to the depth buffer
 * 	C++ Notation:
 *	void setUseDepthMask(bool state);
 */
int LuaMesh::setUseDepthMask(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		if(!lua_isboolean(lua, -1))
		{
			LuaConsole::error(lua, "%s:setUseDepthMask(bool state): state has to be a boolean", className.c_str());
			return 0;
		}

		m_mesh->setUseDepthMask(lua_toboolean(lua, -1));
	}
	else
	{
		LuaConsole::error(lua, "%s:setUseDepthMask(bool state): function requires one parameter", className.c_str());
	}
	return 0;
}

/**
 *  Sets the material of the mesh
 *  C++ Notation:
 *  void setMaterial(Material* material_ptr);
 */
int LuaMesh::setMaterial(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		LuaMaterial* material = LuaClass<LuaMaterial>::check(lua, -1);
		if(material)
		{
			m_mesh->setMaterial(material->getModelObject());
		}
		else
		{
			LuaConsole::error(lua, "%s:setMaterial(Material material): material is not of class type material", className.c_str());
		}
	}
	else
	{
		LuaConsole::error(lua, "%s:setMaterial(Material material): function requires one parameter", className.c_str());
	}
	return 0;
}

int LuaMesh::addChild(lua_State* lua)
{
	LuaMesh* mesh = LuaClass<LuaMesh>::test(lua, -1);
	if(mesh)
	{
		m_node.addChild(mesh->getSceneNode());
	}
	return 0;
}

/**
 *  Returns the model object
 */
MeshRenderer* LuaMesh::getModelObject()
{
	return m_mesh.get();
}

MeshNode* LuaMesh::getSceneNode()
{
	return &m_node;
}

const std::string LuaMesh::className = "Mesh";
const LuaClass<LuaMesh>::Function LuaMesh::functions[] =
{
	{"load", &LuaMesh::load},
	{"loadCube", &LuaMesh::loadCube},
	{"loadPlane", &LuaMesh::loadPlane},

	{"setPosition", &LuaMesh::setPosition},
	{"setScale", &LuaMesh::setScale},
	{"setRotation", &LuaMesh::setRotation},

	{"getPosition", &LuaMesh::getPosition},
	{"getScale", &LuaMesh::getScale},
	{"getRotation", &LuaMesh::getRotation},

	{"setCullFace", &LuaMesh::setCullFace},
	{"setUseDepthMask", &LuaMesh::setUseDepthMask},
	{"setMaterial", &LuaMesh::setMaterial},
	{"addChild", &LuaMesh::addChild},
	{0, 0}
};

const LuaClass<LuaMesh>::Property LuaMesh::properties[] =
{
	{0, 0}
};

int LuaMesh::load_file(const std::string& filename)
{
	if(filename.empty())
	{
		Console::log("Filename '%s' is invalid", filename.c_str());
		return 0;
	}

	if(!m_mesh->load(filename))
	{
		Console::log("Could not load file, '%s' is malformed", filename.c_str());
		return 0;
	}
	return 1;
}
