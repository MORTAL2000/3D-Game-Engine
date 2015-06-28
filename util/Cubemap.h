#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include "ITexture.h"
#include "TextureLoader.h"
#include <core/Console.h>

class Cubemap : public ITexture
{
public:
	Cubemap();

 	bool load(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
	void bind(GLenum);
	void unbind();
	void clear();

private:
	unsigned int m_texture;
};

#endif
