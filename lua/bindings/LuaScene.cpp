#include "LuaScene.h"

#include <Engine.h>
#include <core/Context.h>
#include <util/MaterialLibrary.h>
#include <util/MeshBuilder.h>

#include "LuaMath.h"

/**
 *	LuaScene constructor
 */
LuaScene::LuaScene(lua_State* lua)
{
	Engine* engine = dynamic_cast<Engine*>(Context::getInstance().getCore());
	if(engine)
	{
		m_scene = engine->getScene();
	}
}

LuaScene::~LuaScene()
{}

/**
 *	Adds an entity to the world
 * 	C++ Notation:
 * 	void addEntity(Mesh* mesh);
 *  void addEntity(RigidBody* body);
 */
int LuaScene::addEntity(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		LuaMesh* mesh = LuaClass<LuaMesh>::test(lua, -1);
		LuaRigidBody* body = LuaClass<LuaRigidBody>::test(lua, -1);

		if(mesh)
		{
			m_scene->addChild(mesh->getSceneNode());
		}
		else if(body)
		{
			Engine* engine = dynamic_cast<Engine*>(Context::getInstance().getCore());
			if(engine)
			{
				engine->getPhysicsEngine()->add(body->getModelObject());
			}
		}
		else
		{
			LuaConsole::error(lua, "%s:addEntity(Mesh mesh): mesh is a nil value", className.c_str());
		}
	}
	else
	{
		LuaConsole::error(lua, "%s:addEntity(Mesh mesh): addEntity requires one argument", className.c_str());
	}
	return 0;
}

int LuaScene::removeEntity(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		LuaMesh* mesh = LuaClass<LuaMesh>::test(lua, -1);
		if(mesh)
		{
			m_scene->removeChild(mesh->getSceneNode());
		}
	}
	return 0;
}

/**
 *	Loads a material library
 *  C++ Notation:
 * 	void require(string file);
 */
int LuaScene::require(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		if(lua_isstring(lua, -1))
		{
			MaterialLibrary::getInstance().load(lua_tostring(lua, -1));
		}
	}
	return 0;
}

int LuaScene::loadLevel(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		if(lua_isstring(lua, -1))
		{
			SceneManager::getInstance().load(lua_tostring(lua, -1));
		}
	}
	return 0;
}


/**
 *	Clears the world / removes all objects
 * 	C++ Notation:
 *	void clear();
 */
int LuaScene::clear(lua_State* lua)
{
	if(lua_gettop(lua) != 1)
	{
		LuaConsole::error(lua, "%s:clear(): clear requires no arguments", className.c_str());
		return 0;
	}

	m_scene->clear();
	return 0;
}

int LuaScene::setGravity(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		vec3 gravity = lua_read_vec3(lua, -1);
		Engine* engine = dynamic_cast<Engine*>(Context::getInstance().getCore());
		if(engine)
		{
			engine->getPhysicsEngine()->setGravity(gravity);
		}
	}
	return 0;
}

int LuaScene::getDefaultCamera(lua_State* lua)
{
	Engine* engine = dynamic_cast<Engine*>(Context::getInstance().getCore());
	if(engine)
	{
		FilmCamera* camera = engine->getDefaultCamera();
		LuaClass<LuaCamera>::push(lua, new LuaCamera(camera));
		return 1;
	}
	return 0;
}

int LuaScene::loadMeshes(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		MeshBuilder::getInstance().load(m_scene, lua_tostring(lua, -1));
	}
}

// dev
// enableShadows()
int LuaScene::enableShadows(lua_State* lua)
{
	std::vector<float> resolution = lua_read_vector(lua, -1);
	m_shadow_map.init(resolution[0], resolution[1]);
	return 0;
}

// renderShadows(vec3)
int LuaScene::renderShadows(lua_State* lua)
{
	vec3 sunDirection = lua_read_vec3(lua, -1);
	m_shadow_map.render(sunDirection, m_scene);
	return 0;
}

// passTexture(Material)
int LuaScene::passShadowInfo(lua_State* lua)
{
	LuaMaterial* material_ptr = LuaClass<LuaMaterial>::check(lua, -1);
	if(material_ptr)
	{
		UserMaterial* material = material_ptr->getModelObject();
		m_shadow_map.bindTexture(GL_TEXTURE7);

		Shader* shdr = material->getShader();
		shdr->bind();
		shdr->valuei("hasShadowMap", 1);
		shdr->mat4x4("depthBiasMVP", m_shadow_map.getDepthBias());
		shdr->valuei("shadowMap", 7);
		shdr->unbind();
	}
	return 0;
}


const std::string LuaScene::className = "Scene";
const LuaClass<LuaScene>::Function LuaScene::functions[] =
{
	{"addEntity", &LuaScene::addEntity},
	{"removeEntity", &LuaScene::removeEntity},
	{"require", &LuaScene::require},
	{"load", &LuaScene::loadLevel},
	{"clear", &LuaScene::clear},
	{"setGravity", &LuaScene::setGravity},
	{"getDefaultCamera", &LuaScene::getDefaultCamera},
	{"loadMeshes", &LuaScene::loadMeshes},

	{"enableShadows", &LuaScene::enableShadows},
	{"renderShadows", &LuaScene::renderShadows},
	{"passShadowInfo", &LuaScene::passShadowInfo},
	{0,0}
};

const LuaClass<LuaScene>::Property LuaScene::properties[] =
{
	{0,0}
};
