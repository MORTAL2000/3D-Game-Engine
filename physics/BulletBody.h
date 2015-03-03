#ifndef BULLET_BODY_H
#define BULLET_BODY_H

#include "../core/Commons.h"
#include "../core/Console.h"
#include "../rendering/MeshRenderer.h"
#include "btBulletDynamicsCommon.h"

class BulletBody
{
public:
	BulletBody();
	~BulletBody();

	void construct(btCollisionShape*, const quat&, const vec3&);
	virtual void build(const quat&, const vec3&);

	void setMass(double);
	void setRestitution(double);
	void setFriction(double);

	double getMass();
	double getRestitution();
	double getFriction();

	vec3 getPosition();
	quat getRotation();

	void addForce(const vec3&);
	void addImpulse(const vec3&);
	void addTorque(const vec3&);

	void setLinearVelocity(const vec3&);
	vec3 getLinearVelocity();

	void addRelativeForce(const vec3&);
	vec3 getForwardDirection();

	void setLinearFactor(const vec3&);
	void setAngularFactor(const vec3&);

	void setRenderer(MeshRenderer*);
	MeshRenderer* getRenderer();

	btCollisionShape* getCollisionShape();
	btRigidBody* getRigidBody();
protected:
	double m_mass;
	double m_restitution;
	double m_friction;

	btCollisionShape* m_shape;
	btDefaultMotionState* m_motionState;
	btRigidBody* m_body;
	MeshRenderer* m_renderer;
};

#endif
