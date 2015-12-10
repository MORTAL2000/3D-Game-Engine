/*
 * Copyright 2015 Alexander Koch
 * File: Texture2D.cpp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Texture2D.h"

Texture2D::Texture2D() :
	valid(false), texture(0), width(0), height(0), m_filter(GL_NEAREST), m_bytesPerPixel(GL_RGBA)
{}

Texture2D::Texture2D(const std::string& path) :
	valid(false), texture(0), width(0), height(0), m_filter(GL_NEAREST), m_bytesPerPixel(GL_RGBA)
{
	load(path);
}

Texture2D::~Texture2D() {}

int Texture2D::load(const std::string& filename)
{
	return load(filename, true);
}

int Texture2D::load(const std::string& filename, bool flipYAxis)
{
	glGenTextures(1, &texture);
	vector<unsigned char> data = TextureLoader::getInstance().load(filename, width, height, m_bytesPerPixel);

	if(flipYAxis)
	{
		flipY(data);
	}
	else
	{
		bind(GL_TEXTURE0);
		glTexImage2D(GL_TEXTURE_2D, 0, m_bytesPerPixel, width, height, 0, m_bytesPerPixel, GL_UNSIGNED_BYTE, data.data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);
		unbind();
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	return (valid = true);
}

void Texture2D::flipX(vector<unsigned char>& data)
{
	bind(GL_TEXTURE0);
	flip_horizontal(data, width, height);
	glTexImage2D(GL_TEXTURE_2D, 0, m_bytesPerPixel, width, height, 0, m_bytesPerPixel, GL_UNSIGNED_BYTE, data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);
	unbind();
}

void Texture2D::flipY(vector<unsigned char>& data)
{
	bind(GL_TEXTURE0);
	flip_vertical(data, width, height);
	glTexImage2D(GL_TEXTURE_2D, 0, m_bytesPerPixel, width, height, 0, m_bytesPerPixel, GL_UNSIGNED_BYTE, data.data());
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

void Texture2D::flip_horizontal(vector<unsigned char>& data, unsigned int width, unsigned int height)
{
	vector<unsigned char> new_data;
	unsigned int size = width * height * 4;
	unsigned int stride = sizeof(char) * width * 4;
	new_data.reserve(size);

	for(unsigned int j = 0; j < width; j++)
	{
		unsigned int i = width - j - 1;
		memcpy(new_data.data() + j * stride, data.data() + i * stride, stride);
	}
	memcpy(data.data(), new_data.data(), size);
	new_data.clear();
}

void Texture2D::flip_vertical(vector<unsigned char>& data, unsigned int width, unsigned int height)
{
	vector<unsigned char> new_data;
	unsigned int size = width * height * 4;
	unsigned int stride = sizeof(char) * width * 4;
	new_data.reserve(size);

	for(unsigned int i = 0; i < height; i++)
	{
		unsigned int j = height - i - 1;
		memcpy(new_data.data() + j * stride, data.data() + i * stride, stride);
	}
	memcpy(data.data(), new_data.data(), size);
	new_data.clear();
}
