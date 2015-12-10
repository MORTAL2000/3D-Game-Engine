/*
 * Copyright 2015 Alexander Koch
 * File: BulletBody.cpp
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

#include "BulletBody.h"

BulletBody::BulletBody() :
	m_mass(1), m_restitution(0),
	m_friction(0), m_shape(0), m_motionState(0), m_body(0), m_renderer(0)
{}

BulletBody::~BulletBody()
{
	if(m_body->getMotionState())
	{
		delete m_body->getMotionState();
	}

	delete m_body;
	m_body = 0;

	delete m_shape;
	m_shape = 0;
}

void BulletBody::construct(btCollisionShape* shape, const quat& rotation, const vec3& position)
{
	m_shape = ((shape == NULL) ? ((btCollisionShape*)new btSphereShape(1.0)) : shape);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));
	transform.setRotation(btQuaternion(rotation.w, rotation.x, rotation.y, rotation.z));

	btVector3 inertia(0, 0, 0);
	if(m_mass > 0) {
		m_shape->calculateLocalInertia(m_mass, inertia);
	}

	m_motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(m_mass, m_motionState, m_shape, inertia);
	info.m_restitution = m_restitution;
	info.m_friction = m_friction;

	m_body = new btRigidBody(info);
	m_body->setSleepingThresholds(0.2f, 0.2f);

	// If mass is less than zero, set it to static
	if(m_mass <= 0) {
		m_mass = 0;
		m_body->setCollisionFlags(m_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		m_body->setActivationState(DISABLE_DEACTIVATION);
	}
}

void BulletBody::build(const quat& roation, const vec3& position)
{
	Console::log("::Pure virtual method called::");
}

void BulletBody::setMass(double mass)
{
	m_mass = mass;
}

void BulletBody::setRestitution(double restitution)
{
	m_restitution = restitution;
}

void BulletBody::setFriction(double friction)
{
	m_friction = friction;
}

double BulletBody::getMass()
{
	return m_mass;
}

double BulletBody::getRestitution()
{
	return m_restitution;
}

double BulletBody::getFriction()
{
	return m_friction;
}

vec3 BulletBody::getPosition()
{
	vec3 position;
	if(m_body)
	{
		btTransform transform;
		m_body->getMotionState()->getWorldTransform(transform);
		btVector3 v = transform.getOrigin();
		position = vec3(v.getX(), v.getY(), v.getZ());
	}
	return position;
}

quat BulletBody::getRotation()
{
	quat rotation;
	if(m_body)
	{
		btTransform transform;
		m_body->getMotionState()->getWorldTransform(transform);
		btQuaternion r = transform.getRotation();
		rotation = quat(r.getW(), r.getX(), r.getY(), r.getZ());
	}
	return rotation;
}

void BulletBody::addForce(const vec3& force)
{
	m_body->activate(true);
	m_body->applyCentralForce(btVector3(force.x, force.y, force.z));
}

void BulletBody::addImpulse(const vec3& impulse)
{
	m_body->activate(true);
	m_body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
}

void BulletBody::addTorque(const vec3& torque)
{
	m_body->activate(true);
	m_body->applyTorque(btVector3(torque.x, torque.y, torque.z));
}

void BulletBody::setLinearVelocity(const vec3& velocity)
{
	m_body->activate(true);
	m_body->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

vec3 BulletBody::getLinearVelocity()
{
	btVector3 velocity = m_body->getLinearVelocity();
	return vec3(velocity.getX(), velocity.getY(), velocity.getZ());
}

void BulletBody::addRelativeForce(const vec3& force)
{
	btVector3 relativeForce = btVector3(force.x, force.y, force.z);
	btVector3 correctedForce = m_body->getWorldTransform().getBasis() * relativeForce;
	m_body->applyCentralForce(correctedForce);
}

vec3 BulletBody::getForwardDirection()
{
	btVector3 dir = m_body->getWorldTransform().getBasis().getColumn(2);
	return vec3(dir.getX(), dir.getY(), dir.getZ());
}

void BulletBody::setLinearFactor(const vec3& linear)
{
	m_body->setLinearFactor(btVector3(linear.x, linear.y, linear.z));
}

void BulletBody::setAngularFactor(const vec3& angular)
{
	m_body->setAngularFactor(btVector3(angular.x, angular.y, angular.z));
}

void BulletBody::setRenderer(MeshRenderer* renderer)
{
	m_renderer = renderer;
}

MeshRenderer* BulletBody::getRenderer()
{
	return m_renderer;
}

btCollisionShape* BulletBody::getCollisionShape()
{
	return m_shape;
}

btRigidBody* BulletBody::getRigidBody()
{
	return m_body;
}
