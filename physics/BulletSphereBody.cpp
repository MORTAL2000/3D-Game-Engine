#include "BulletSphereBody.h"

BulletSphereBody::BulletSphereBody() : m_shape(0), m_radius(1.0)
{

}

BulletSphereBody::~BulletSphereBody()
{
	if(m_shape)
	{
		delete m_shape;
		m_shape = 0;
	}
}

void BulletSphereBody::setRadius(double radius)
{
	m_radius = radius;
}

double BulletSphereBody::getRadius()
{
	return m_radius;
}

void BulletSphereBody::build(const quat& rotation, const vec3& position)
{
	m_shape = new btSphereShape(m_radius);
	BulletBody::construct(m_shape, rotation, position);
}
