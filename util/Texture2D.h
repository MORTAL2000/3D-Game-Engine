/**
 * @file Texture2D.h
 * @author Alexander Koch
 */

#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <stdio.h>
#include <string>

#include "../core/Commons.h"
#include "../core/Tokenizer.h"
#include "../core/FileReader.h"
#include "../core/Console.h"
#include "ITexture.h"
#include "TextureLoader.h"

class Texture2D : public ITexture
{
public:
	Texture2D();
	Texture2D(const std::string&);
	~Texture2D();

	int load(const std::string&);
	int load(const std::string&, bool);
	void flipX(unsigned char*);
	void flipY(unsigned char*);

	void bind(GLenum);
	void unbind();

	void setFiltering(GLenum);
	GLenum getFiltering();

	int getWidth();
	int getHeight();
	bool isValid();

	void clear();
private:
	bool valid;
	unsigned int texture;
	unsigned int width;
	unsigned int height;
	GLenum m_filter;
	GLenum m_bytesPerPixel;

	void flip_vertical(unsigned char*, unsigned int, unsigned int);
	void flip_horizontal(unsigned char*, unsigned int, unsigned int);
};

#endif
