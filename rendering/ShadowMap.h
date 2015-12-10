/*
 * Copyright 2015 Alexander Koch
 * File: ShadowMap.h
 * Description: Class for using shadow maps.
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

#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <core/Commons.h>
#include <core/Console.h>
#include <scene/SceneNode.h>
#include <util/Shader.h>

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void init(int, int);

	void bind();
	void unbind();
	void render(const vec3&, SceneNode*);

	void bindTexture(GLenum);
	void unbindTexture();

	mat4 getDepthBias();
private:
	Shader m_shader;
	unsigned int buffer;
	unsigned int texture;
	vec2 dimension;
	mat4 mvp;

	void check();
};

#endif
