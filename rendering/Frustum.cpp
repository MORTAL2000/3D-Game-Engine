#include "Frustum.h"

Frustum::Frustum()
{

}

Frustum::Frustum(const mat4& viewMatrix)
{
	calculate(viewMatrix);
}

void Frustum::calculate(const mat4& viewMatrix)
{
	m_planes[0][0] = viewMatrix[0][3] + viewMatrix[0][0];
	m_planes[0][1] = viewMatrix[1][3] + viewMatrix[1][0];
	m_planes[0][2] = viewMatrix[2][3] + viewMatrix[2][0];
	m_planes[0][3] = viewMatrix[3][3] + viewMatrix[3][0];

    m_planes[1][0] = viewMatrix[0][3] - viewMatrix[0][0];
    m_planes[1][1] = viewMatrix[1][3] - viewMatrix[1][0];
    m_planes[1][2] = viewMatrix[2][3] - viewMatrix[2][0];
    m_planes[1][3] = viewMatrix[3][3] - viewMatrix[3][0];

    m_planes[2][0] = viewMatrix[0][3] - viewMatrix[0][1];
    m_planes[2][1] = viewMatrix[1][3] - viewMatrix[1][1];
    m_planes[2][2] = viewMatrix[2][3] - viewMatrix[2][1];
    m_planes[2][3] = viewMatrix[3][3] - viewMatrix[3][1];

    m_planes[3][0] = viewMatrix[0][3] + viewMatrix[0][1];
    m_planes[3][1] = viewMatrix[1][3] + viewMatrix[1][1];
    m_planes[3][2] = viewMatrix[2][3] + viewMatrix[2][1];
    m_planes[3][3] = viewMatrix[3][3] + viewMatrix[3][1];

    m_planes[4][0] = viewMatrix[0][3] + viewMatrix[0][2];
    m_planes[4][1] = viewMatrix[1][3] + viewMatrix[1][2];
    m_planes[4][2] = viewMatrix[2][3] + viewMatrix[2][2];
    m_planes[4][3] = viewMatrix[3][3] + viewMatrix[3][2];

    m_planes[5][0] = viewMatrix[0][3] - viewMatrix[0][2];
    m_planes[5][1] = viewMatrix[1][3] - viewMatrix[1][2];
    m_planes[5][2] = viewMatrix[2][3] - viewMatrix[2][2];
    m_planes[5][3] = viewMatrix[3][3] - viewMatrix[3][2];

	for(int i = 0; i < 6; i++)
	{
        float invl = sqrt(m_planes[i].x * m_planes[i].x +
                          m_planes[i].y * m_planes[i].y +
                          m_planes[i].z * m_planes[i].z);
        m_planes[i] /= invl;
    }
}

bool Frustum::boxInFrustum(const BoundingBox& bbox)
{
	vec3 min = bbox.getMin(), max = bbox.getMax();

	for(int i = 0; i < 6; i++)
    {
        int out = 0;
        out += ((dot(m_planes[i], vec4(min.x, min.y, min.z, 1.0f)) < 0.0) ? 1:0);
        out += ((dot(m_planes[i], vec4(max.x, min.y, min.z, 1.0f)) < 0.0) ? 1:0);
        out += ((dot(m_planes[i], vec4(min.x, max.y, min.z, 1.0f)) < 0.0) ? 1:0);
        out += ((dot(m_planes[i], vec4(max.x, max.y, min.z, 1.0f)) < 0.0) ? 1:0);
        out += ((dot(m_planes[i], vec4(min.x, min.y, max.z, 1.0f)) < 0.0) ? 1:0);
        out += ((dot(m_planes[i], vec4(max.x, min.y, max.z, 1.0f)) < 0.0) ? 1:0);
        out += ((dot(m_planes[i], vec4(min.x, max.y, max.z, 1.0f)) < 0.0) ? 1:0);
        out += ((dot(m_planes[i], vec4(max.x, max.y, max.z, 1.0f)) < 0.0) ? 1:0);
        if(out == 8) return false;
    }

	return true;
}
