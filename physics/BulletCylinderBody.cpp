#include "BulletCylinderBody.h"

BulletCylinderBody::BulletCylinderBody() : m_shape(0), m_half_extent(1.0)
{

}

BulletCylinderBody::~BulletCylinderBody()
{

}

void BulletCylinderBody::setHalfExtent(const vec3& extent)
{
    m_half_extent = extent;
}

vec3 BulletCylinderBody::getHalfExtent()
{
    return m_half_extent;
}

void BulletCylinderBody::build(const quat& rotation, const vec3& position)
{
    m_shape = new btCylinderShape(btVector3(m_half_extent.x, m_half_extent.y, m_half_extent.z));
    BulletBody::construct(m_shape, rotation, position);
}
