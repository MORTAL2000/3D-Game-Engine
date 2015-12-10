/*
 * Copyright 2015 Alexander Koch
 * File: BulletPhysicsEngine.h
 * Description: Wrapper for the bullet physics engine.
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

#ifndef BULLET_PHYSICS_ENGINE_H
#define BULLET_PHYSICS_ENGINE_H

#include <core/Commons.h>
#include <core/Console.h>
#include "btBulletDynamicsCommon.h"
#include "BulletBody.h"
#include "BulletSphereBody.h"
#include "BulletMeshBody.h"
#include "BulletBoxBody.h"
#include <vector>

class BulletPhysicsEngine
{
public:
	BulletPhysicsEngine();

	void initialize();
	void finalize();

	void add(BulletBody*);
	void remove(BulletBody*);

	bool checkSphere(const vec3&, const vec3&, float);

	void setGravity(const vec3&);
	void step();
private:
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btBroadphaseInterface* m_broadphase;
	btConstraintSolver* m_solver;
	btDynamicsWorld* m_dynamicsWorld;

	std::vector<BulletBody*> m_entries;
};

#endif
