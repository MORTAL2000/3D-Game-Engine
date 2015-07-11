#include "BoundingBox.h"

BoundingBox::BoundingBox()
{}

BoundingBox::BoundingBox(const vec3& min, const vec3& max) : m_min(min), m_max(max)
{}

void BoundingBox::set(const vec3& min, const vec3& max)
{
	m_min = min;
	m_max = max;
}

vec3 BoundingBox::getMin() const
{
	return m_min;
}

vec3 BoundingBox::getMax() const
{
	return m_max;
}
