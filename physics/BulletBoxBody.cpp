#include "BulletBoxBody.h"

BulletBoxBody::BulletBoxBody() : m_shape(0), m_scale(1.0)
{

}

BulletBoxBody::~BulletBoxBody()
{
	delete m_shape;
	m_shape = 0;
}

void BulletBoxBody::setScale(const vec3& scale)
{
	m_scale = scale;
}

vec3 BulletBoxBody::getScale()
{
	return m_scale;
}

void BulletBoxBody::build(const quat& rotation, const vec3& position)
{
	m_shape = new btBoxShape(btVector3(m_scale.x, m_scale.y, m_scale.z));
	BulletBody::construct(m_shape, rotation, position);
}
