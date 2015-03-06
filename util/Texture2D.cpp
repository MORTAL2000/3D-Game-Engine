#include "Texture2D.h"

Texture2D::Texture2D() :
	valid(false), texture(0), width(0), height(0), m_filter(GL_NEAREST), m_bytesPerPixel(GL_RGBA)
{}

Texture2D::Texture2D(const std::string& path) :
	valid(false), texture(0), width(0), height(0), m_filter(GL_NEAREST), m_bytesPerPixel(GL_RGBA)
{
	load(path);
}

Texture2D::~Texture2D()
{}

int Texture2D::load(const std::string& filename)
{
	return load(filename, true);
}

int Texture2D::load(const std::string& filename, bool flipYAxis)
{
	glGenTextures(1, &texture);
	unsigned char* data = TextureLoader::load(filename, width, height, m_bytesPerPixel);

	if(flipYAxis)
	{
		flipY(data);
	}
	else
	{
		bind(GL_TEXTURE0);
		glTexImage2D(GL_TEXTURE_2D, 0, m_bytesPerPixel, width, height, 0, m_bytesPerPixel, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);
		unbind();
	}
	delete[] data;
	return (valid = true);
}

void Texture2D::flipX(unsigned char* data)
{
	bind(GL_TEXTURE0);
	flip_horizontal(data, width, height);
	glTexImage2D(GL_TEXTURE_2D, 0, m_bytesPerPixel, width, height, 0, m_bytesPerPixel, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);
	unbind();
}

void Texture2D::flipY(unsigned char* data)
{
	bind(GL_TEXTURE0);
	flip_vertical(data, width, height);
	glTexImage2D(GL_TEXTURE_2D, 0, m_bytesPerPixel, width, height, 0, m_bytesPerPixel, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);
	unbind();
}

void Texture2D::bind(GLenum unit)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture2D::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void Texture2D::setFiltering(GLenum filter)
{
	m_filter = filter;
}

GLenum Texture2D::getFiltering()
{
	return m_filter;
}

int Texture2D::getWidth()
{
	return int(width);
}

int Texture2D::getHeight()
{
	return int(height);
}

bool Texture2D::isValid()
{
	return valid;
}

void Texture2D::clear()
{
	glDeleteTextures(1, &texture);
	valid = false;
}

void Texture2D::flip_horizontal(unsigned char* data, unsigned int width, unsigned int height)
{
	unsigned int size = width * height * 4;
	unsigned int stride = sizeof(char) * width * 4;
	unsigned char *new_data = new unsigned char[size];
	if(!new_data)
	{
		Console::log("Memory error");
		return;
	}

	for(unsigned int j = 0; j < width; j++)
	{
		unsigned int i = width - j - 1;
		memcpy(new_data + j * stride, data + i * stride, stride);
	}
	memcpy(data, new_data, size);
	delete[] new_data;
}

void Texture2D::flip_vertical(unsigned char *data, unsigned int width, unsigned int height)
{
	unsigned int size = width * height * 4;
	unsigned int stride = sizeof(char) * width * 4;
	unsigned char *new_data = new unsigned char[size];
	if(!new_data)
	{
		Console::log("Memory error");
		return;
	}

	for(unsigned int i = 0; i < height; i++)
	{
		unsigned int j = height - i - 1;
		memcpy(new_data + j * stride, data + i * stride, stride);
	}
	memcpy(data, new_data, size);
	delete[] new_data;
}
