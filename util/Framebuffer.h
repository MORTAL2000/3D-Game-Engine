#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "../core/Commons.h"
#include "../core/Console.h"

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void initialize(int, int);

	void bind();
	void unbind();
	void bindTexture(unsigned int, GLenum);
	void unbindTexture();

	void checkValidity();
private:
	unsigned int m_framebuffer;
 	unsigned int m_depthTexture;
	unsigned int m_texture;
};

#endif
