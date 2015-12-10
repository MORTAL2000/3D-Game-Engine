/*
 * Copyright 2015 Alexander Koch
 * File: Cubemap.h
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
