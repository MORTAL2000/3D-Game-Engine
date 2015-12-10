#ifndef CUSTOM_CAMERA_H
#define CUSTOM_CAMERA_H

#include <util/FilmCamera.h>

class CustomCamera : public FilmCamera
{
public:
	CustomCamera();

	void setModelViewProjection(const mat4&);
	void update();

	mat4 getViewMatrix();
	mat4 getProjectionMatrix();
private:
	mat4 m_mvp;
};

#endif
