#ifndef DEBUG_H
#define DEBUG_H

#include <math.h>
#include <core/Commons.h>
#include <util/Shader.h>

namespace gl
{
	void init();
	void update(const mat4&);
	void setColor(const vec4&);
	void rotate(float, const vec3&);
	void translate(const vec3&);

	void viewport(const vec4&);
	void clearColor(const vec4&);
	void clear();

	void begin();
	void end();
	void drawLine(const vec2&, const vec2&);
	void drawLine(const vec3&, const vec3&);
	void drawWireCube(const vec3&, const vec3&);
	void drawVector(const vec3&, const vec3&, float, float);

	mat4 getModelViewProjection();
}

#endif
