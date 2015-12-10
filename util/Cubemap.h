#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include "ITexture.h"
#include "TextureLoader.h"
#include <core/Console.h>

using std::string;

class Cubemap : public ITexture
{
public:
	Cubemap();

 	bool load(const string&, const string&, const string&, const string&, const string&, const string&);
	void bind(GLenum);
	void unbind();
	void clear();

private:
	unsigned int m_texture;
};

#endif
