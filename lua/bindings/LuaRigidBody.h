/*
 * Copyright 2015 Alexander Koch
 * File: LuaRigidBody.h
 * Description: Rigid-body represenation in lua.
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
