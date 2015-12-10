/*
 * Copyright 2015 Alexander Koch
 * File: Cubemap.cpp
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

#include "Cubemap.h"

Cubemap::Cubemap() {}

bool Cubemap::load(const string& xpos, const string& xneg, const string& ypos, const string& yneg, const string& zpos, const string& zneg)
{
	glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// HACK assuming that all textures have the same size, fix
	unsigned int width = 0, height = 0, bpp = 4;
	vector<unsigned char> xPos = TextureLoader::getInstance().load(xpos, width, height, bpp);
	vector<unsigned char> xNeg = TextureLoader::getInstance().load(xneg, width, height, bpp);
	vector<unsigned char> yPos = TextureLoader::getInstance().load(ypos, width, height, bpp);
	vector<unsigned char> yNeg = TextureLoader::getInstance().load(yneg, width, height, bpp);
	vector<unsigned char> zPos = TextureLoader::getInstance().load(zpos, width, height, bpp);
	vector<unsigned char> zNeg = TextureLoader::getInstance().load(zneg, width, height, bpp);

	if(xPos.empty() || xNeg.empty() || yPos.empty() || yNeg.empty() || zPos.empty() || zNeg.empty())
	{
		Console::log("Error loading the cubemap textures");
		return false;
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, xPos.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, xNeg.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, yPos.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, yNeg.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, zPos.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, zNeg.data());

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	return true;
}

void Cubemap::bind(GLenum unit)
{
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}

void Cubemap::unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);
}

void Cubemap::clear()
{
	glDeleteTextures(1, &m_texture);
}
