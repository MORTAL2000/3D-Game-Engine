/*
 * Copyright 2015 Alexander Koch
 * File: TextureLibrary.h
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

#ifndef TEXTURE_LIBRARY_H
#define TEXTURE_LIBRARY_H

#include <vector>
#include <map>
#include <string>

#include "ITexture.h"
#include "Texture2D.h"
#include "Cubemap.h"

typedef std::map<std::string, Texture2D> Texture2DCache;
typedef std::map<std::string, Cubemap> CubemapCache;

class TextureLibrary
{
public:
	static TextureLibrary& getInstance();

	bool load(const std::string&, const std::string&);
	bool load(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

	bool containsTexture2D(const std::string&);
	bool containsCubemap(const std::string&);
	Texture2D* getTexture2D(const std::string&);
	Cubemap* getCubemap(const std::string&);
	ITexture* get(const std::string&);

	void free();
private:
	TextureLibrary();
 	Texture2DCache m_tex2d_cache;
	CubemapCache m_cube_cache;
	long m_texture_count;
};

#endif
