#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <core/Commons.h>
#include <rendering/BoundingBox.h>

class Frustum
{
public:
	Frustum();
	Frustum(const mat4& viewMatrix);

	void calculate(const mat4& viewMatrix);
	bool boxInFrustum(const BoundingBox& bbox);

private:
	vec4 m_planes[6];
};

#endif
