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
	Texture2D(const string& path);
	~Texture2D();

	int load(const string& filename);
	int load(const string& filename, bool flipYAxis);
	void flipX(vector<unsigned char>& data);
	void flipY(vector<unsigned char>& data);

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

	void flip_vertical(vector<unsigned char>& data, unsigned int, unsigned int);
	void flip_horizontal(vector<unsigned char>& data, unsigned int, unsigned int);
};

#endif
