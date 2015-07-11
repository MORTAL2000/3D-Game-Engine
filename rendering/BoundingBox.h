#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <core/Commons.h>

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(const vec3& min, const vec3& max);

	void set(const vec3& min, const vec3& max);

	vec3 getMin() const;
	vec3 getMax() const;

private:
	vec3 m_min;
	vec3 m_max;
};

#endif
