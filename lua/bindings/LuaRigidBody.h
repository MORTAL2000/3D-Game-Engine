#ifndef LUA_RIGID_BODY_H
#define LUA_RIGID_BODY_H

#include <memory>

#include <lua/LuaBase.h>
#include <lua/LuaClass.h>
#include <lua/LuaConsole.h>
#include <lua/LuaEnums.h>
#include "LuaMesh.h"

#include <physics/BulletBody.h>
#include <physics/BulletSphereBody.h>
#include <physics/BulletMeshBody.h>
#include <physics/BulletBoxBody.h>
#include <physics/BulletCylinderBody.h>

class LuaRigidBody : public LuaBase
{
public:
	enum Shape { TYPE_SPHERE=0, TYPE_BOX, TYPE_CYLINDER, TYPE_MESH };

	LuaRigidBody(lua_State*);
	~LuaRigidBody();

	int setMass(lua_State*);
	int setFriction(lua_State*);
	int setRestitution(lua_State*);

	int setLinearFactor(lua_State*);
	int setAngularFactor(lua_State*);

	int create(lua_State*);
	int setMesh(lua_State*);

	int addForce(lua_State*);
	int addImpulse(lua_State*);
	int addTorque(lua_State*);

	int setLinearVelocity(lua_State*);
	int getLinearVelocity(lua_State*);

	int addRelativeForce(lua_State*);
	int getForwardDirection(lua_State*);

	int getPosition(lua_State*);

	BulletBody* getModelObject();
	static const std::string className;
	static const LuaClass<LuaRigidBody>::Function functions[];
	static const LuaClass<LuaRigidBody>::Property properties[];
private:
	std::shared_ptr<BulletBody> m_bullet_body;
	int m_type;
};

#endif
