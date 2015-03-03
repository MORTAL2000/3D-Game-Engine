#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include "../core/Commons.h"
#include "../core/Console.h"
#include "../scene/SceneNode.h"
#include "../util/Shader.h"

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void init(int, int);

	void bind();
	void unbind();
	void render(const vec3&, SceneNode*);

	void bindTexture(GLenum);
	void unbindTexture();

	mat4 getDepthBias();
private:
	Shader m_shader;
	unsigned int buffer;
	unsigned int texture;
	vec2 dimension;
	mat4 mvp;

	void check();
};

#endif
