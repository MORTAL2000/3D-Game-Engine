#ifndef BULLET_PHYSICS_ENGINE_H
#define BULLET_PHYSICS_ENGINE_H

#include "../core/Commons.h"
#include "../core/Console.h"
#include "btBulletDynamicsCommon.h"
#include "BulletBody.h"
#include "BulletSphereBody.h"
#include "BulletMeshBody.h"
#include "BulletBoxBody.h"

#include <vector>

/**
 * Wrapper for the bullet physics engine
 */
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
