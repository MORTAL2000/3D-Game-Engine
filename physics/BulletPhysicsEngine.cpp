#include "BulletPhysicsEngine.h"

BulletPhysicsEngine::BulletPhysicsEngine() : m_dynamicsWorld(0), m_solver(0), m_broadphase(0), m_dispatcher(0), m_collisionConfiguration(0)
{}

void BulletPhysicsEngine::initialize()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_broadphase = new btDbvtBroadphase();
	m_solver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	Console::log("Physics: Done initializing bullet physics.");
}

void BulletPhysicsEngine::add(BulletBody* body)
{
	m_entries.push_back(body);
	m_dynamicsWorld->addRigidBody(body->getRigidBody());
}

void BulletPhysicsEngine::remove(BulletBody* body)
{
	for(auto i = 0; i < m_entries.size(); i++)
	{
		if(m_entries[i] == body)
		{
			m_dynamicsWorld->removeRigidBody(m_entries[i]->getRigidBody());
			m_entries.erase(m_entries.begin() + i);
			break;
		}
	}
}

bool BulletPhysicsEngine::checkSphere(const vec3& start, const vec3& end, float radius)
{
	btTransform origin, target;
	origin.setIdentity();
	target.setIdentity();
	origin.setOrigin(btVector3(start.x, start.y, start.z));
	target.setOrigin(btVector3(end.x, end.y, end.z));

	btCollisionWorld::ClosestConvexResultCallback result(btVector3(start.x, start.y, start.z), btVector3(end.x, end.y, end.z));
	btSphereShape shape(radius);
	m_dynamicsWorld->convexSweepTest(&shape, origin, target, result);
	return result.hasHit();
}

void BulletPhysicsEngine::finalize()
{
	for(auto i = 0; i < m_entries.size(); i++)
	{
		auto body = m_entries[i]->getRigidBody();
		if(body) m_dynamicsWorld->removeRigidBody(body);
	}

	delete m_dynamicsWorld;
	delete m_solver;
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_broadphase;

	m_dynamicsWorld = 0;
	m_solver = 0;
	m_broadphase = 0;
	m_dispatcher = 0;
	m_collisionConfiguration = 0;

	Console::log("Physics: Successfully destroyed bullet physics components.");
}

void BulletPhysicsEngine::setGravity(const vec3& gravity)
{
	m_dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void BulletPhysicsEngine::step()
{
	if(!m_dynamicsWorld) return;
	m_dynamicsWorld->stepSimulation(1.0f / 60.0f);

	for(auto i = 0; i < m_entries.size(); i++)
	{
		if(!m_entries[i]) continue;
		MeshRenderer* renderer = m_entries[i]->getRenderer();
		btRigidBody* body = m_entries[i]->getRigidBody();

		if(renderer && body)
		{
			btTransform transform;
			body->getMotionState()->getWorldTransform(transform);
			btVector3 position = transform.getOrigin();
			btQuaternion rotation = transform.getRotation();

			renderer->setPosition(vec3(position.getX(), position.getY(), position.getZ()));
			renderer->setRotation(quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ()));
		}
	}
}
