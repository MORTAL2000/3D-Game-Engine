#include "LuaRigidBody.h"
#include "LuaMath.h"

LuaRigidBody::LuaRigidBody(lua_State* lua)
{
	int args = lua_gettop(lua);
	if(check_enum_type(lua, "Shape", -args))
	{
		m_type = get_enum_value(lua, -args);

		switch(m_type)
		{
			case TYPE_SPHERE:
			{
				std::shared_ptr<BulletSphereBody> sphere(new BulletSphereBody());
				if(args == 2)
				{
					sphere->setRadius(lua_tonumber(lua, -1));
				}

				m_bullet_body = std::move(sphere);
				break;
			}
			case TYPE_MESH:
			{
				std::shared_ptr<BulletMeshBody> mesh(new BulletMeshBody());
				m_bullet_body = std::move(mesh);
				break;
			}
			case TYPE_BOX:
			{
				std::shared_ptr<BulletBoxBody> box(new BulletBoxBody());
				if(args == 2)
				{
					box->setScale(lua_read_vec3(lua, -1));
				}
				m_bullet_body = std::move(box);
				break;
			}
			case TYPE_CYLINDER:
			{
				std::shared_ptr<BulletCylinderBody> cylinder(new BulletCylinderBody());
				if(args == 2)
				{
					cylinder->setHalfExtent(lua_read_vec3(lua, -1));
				}
				m_bullet_body = std::move(cylinder);
				break;
			}
		}
	}
	else
	{
		m_type = TYPE_SPHERE;
		std::shared_ptr<BulletSphereBody> sphere(new BulletSphereBody());
		m_bullet_body = std::move(sphere);
	}
}

LuaRigidBody::~LuaRigidBody()
{

}

int LuaRigidBody::setMass(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		if(lua_isnumber(lua, -1))
		{
			m_bullet_body->setMass(lua_tonumber(lua, -1));
		}
	}
	return 0;
}

int LuaRigidBody::setFriction(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		if(lua_isnumber(lua, -1))
		{
			m_bullet_body->setFriction(lua_tonumber(lua, -1));
		}
	}
	return 0;
}

int LuaRigidBody::setRestitution(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		if(lua_isnumber(lua, -1))
		{
			m_bullet_body->setRestitution(lua_tonumber(lua, -1));
		}
	}
	return 0;
}

int LuaRigidBody::setLinearFactor(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		m_bullet_body->setLinearFactor(lua_read_vec3(lua, -1));
	}
	return 0;
}

int LuaRigidBody::setAngularFactor(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		m_bullet_body->setAngularFactor(lua_read_vec3(lua, -1));
	}
	return 0;
}

/**
 *	Constructs a Bullet Body
 *  void create(vec4, vec3)
 */
int LuaRigidBody::create(lua_State* lua)
{
	if(lua_gettop(lua) == 3)
	{
		vec3 position = lua_read_vec3(lua, -2);
		vec4 rotation = lua_read_vec4(lua, -1);
		m_bullet_body->build(quat(rotation.x, rotation.y, rotation.z, rotation.w), position);
	}
	return 0;
}

/**
 *	Sets the mesh renderer of the body
 */
int LuaRigidBody::setMesh(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		LuaMesh* mesh = LuaClass<LuaMesh>::check(lua, -1);
		if(mesh)
		{
			m_bullet_body->setRenderer(mesh->getModelObject());
		}
		else
		{
			LuaConsole::error(lua, "%s:addEntity(Mesh mesh): mesh is a nil value", className.c_str());
		}
	}
	else
	{
		LuaConsole::error(lua, "%s:setMesh(Mesh mesh): setMesh requires one argument", className.c_str());
	}
	return 0;
}

/**
 *	Adds a force to the body
 */
int LuaRigidBody::addForce(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		m_bullet_body->addForce(lua_read_vec3(lua, -1));
	}
	else
	{
		LuaConsole::error(lua, "%s:addForce(vec3 force): function requires one argument", className.c_str());
	}
	return 0;
}

/**
*	Adds an impulse to the body
*/
int LuaRigidBody::addImpulse(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		m_bullet_body->addImpulse(lua_read_vec3(lua, -1));
	}
	else
	{
		LuaConsole::error(lua, "%s:addImpulse(vec3 impulse): function requires one argument", className.c_str());
	}
	return 0;
}

int LuaRigidBody::addTorque(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		m_bullet_body->addTorque(lua_read_vec3(lua, -1));
	}
	else
	{
		LuaConsole::error(lua, "%s:addTorque(vec3 torque): function requires one argument", className.c_str());
	}
	return 0;
}

int LuaRigidBody::setLinearVelocity(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		m_bullet_body->setLinearVelocity(lua_read_vec3(lua, -1));
	}
	else
	{
		LuaConsole::error(lua, "%s:setLinearVelocity(vec3 velocity): function requires one argument", className.c_str());
	}
	return 0;
}

int LuaRigidBody::getLinearVelocity(lua_State* lua)
{
	lua_push_vec3(lua, m_bullet_body->getLinearVelocity());
	return 1;
}

int LuaRigidBody::addRelativeForce(lua_State* lua)
{
	if(lua_gettop(lua) == 2)
	{
		m_bullet_body->addRelativeForce(lua_read_vec3(lua, -1));
	}
	else
	{
		LuaConsole::error(lua, "%s:addRelativeForce(vec3 forcce): functions requires one argument", className.c_str());
	}
	return 0;
}

int LuaRigidBody::getForwardDirection(lua_State* lua)
{
	if(lua_gettop(lua) == 1)
	{
		vec3 dir = m_bullet_body->getForwardDirection();
		lua_push_vec3(lua, dir);
		return 1;
	}
	else
	{
		LuaConsole::error(lua, "%s:getForwardDirection(): function requires no arguments", className.c_str());
	}
	return 0;
}

int LuaRigidBody::getPosition(lua_State* lua)
{
	lua_push_vec3(lua, m_bullet_body->getPosition());
	return 1;
}

BulletBody* LuaRigidBody::getModelObject()
{
	return m_bullet_body.get();
}

const std::string LuaRigidBody::className = "RigidBody";
const LuaClass<LuaRigidBody>::Function LuaRigidBody::functions[] =
{
	{"setMass", &LuaRigidBody::setMass},
	{"setFriction", &LuaRigidBody::setFriction},
	{"setRestitution", &LuaRigidBody::setRestitution},
	{"setLinearFactor", &LuaRigidBody::setLinearFactor},
	{"setAngularFactor", &LuaRigidBody::setAngularFactor},
	{"create", &LuaRigidBody::create},
	{"setMesh", &LuaRigidBody::setMesh},
	{"addForce", &LuaRigidBody::addForce},
	{"addImpulse", &LuaRigidBody::addImpulse},
	{"addTorque", &LuaRigidBody::addTorque},
	{"setLinearVelocity", &LuaRigidBody::setLinearVelocity},
	{"getLinearVelocity", &LuaRigidBody::getLinearVelocity},
	{"addRelativeForce", &LuaRigidBody::addRelativeForce},
	{"getForwardDirection", &LuaRigidBody::getForwardDirection},
	{"getPosition", &LuaRigidBody::getPosition},
	{0,0}
};

const LuaClass<LuaRigidBody>::Property LuaRigidBody::properties[] =
{
	{0,0}
};
