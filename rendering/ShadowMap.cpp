#include "ShadowMap.h"

ShadowMap::ShadowMap() {}

ShadowMap::~ShadowMap()
{
	glDeleteFramebuffers(1, &buffer);
	glDeleteTextures(1, &texture);
}

void ShadowMap::init(int width, int height)
{
	dimension = vec2(width, height);
	glGenFramebuffers(1, &buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	#ifdef PCF
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	#endif

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	glDrawBuffer(GL_NONE);

	check();

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_shader.load("shaders/shadow.vsh", "shaders/shadow.fsh");
}

void ShadowMap::bind()
{
	glEnable(GL_FRAMEBUFFER);
	glViewport(0, 0, dimension.x, dimension.y);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
}

void ShadowMap::unbind()
{
	glDisable(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::render(const vec3& direction, SceneNode* scene)
{
	bind();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	float size = 110.0f;
	float near = -size;
	float far = size;

	mat4 projectionMatrix = glm::ortho(-size, size, -size, size, near, far);
	mat4 viewMatrix = glm::lookAt(direction, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
	m_shader.bind();
	m_shader.mat4x4("mvp", projectionMatrix * viewMatrix);
	m_shader.unbind();

	scene->draw(mat4(1.0), &m_shader);
	unbind();

	mvp = projectionMatrix * viewMatrix;
}

void ShadowMap::bindTexture(GLenum unit)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void ShadowMap::unbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

mat4 ShadowMap::getDepthBias()
{
	mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	return biasMatrix * mvp;
}

void ShadowMap::check()
{
	unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		switch(status)
		{
			case GL_FRAMEBUFFER_UNSUPPORTED:
				Console::log("Unsupported framebuffer format");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				Console::log("Framebuffer incomplete, missing attachment");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				Console::log("Framebuffer incomplete, missing draw buffer");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				Console::log("Framebuffer incomplete, missing read buffer");
				break;
			default:
				Console::log("Unknown Framebuffer error");
				break;
		}
	}
}
