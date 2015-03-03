#include "CustomCamera.h"

CustomCamera::CustomCamera()
{

}

void CustomCamera::setModelViewProjection(const mat4& mvp)
{
	m_mvp = mvp;
	m_projectionMatrix = m_mvp;
}

void CustomCamera::update()
{
	// do nothing
}

mat4 CustomCamera::getViewMatrix()
{
	return mat4(1.0);
}

mat4 CustomCamera::getProjectionMatrix()
{
	return m_mvp;
}
