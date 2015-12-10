/*
 * Copyright 2015 Alexander Koch
 * File: GL.h
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

#ifndef DEBUG_H
#define DEBUG_H

#include <math.h>
#include <core/Commons.h>
#include <util/Shader.h>

namespace gl
{
	void init();
	void update(const mat4&);
	void setColor(const vec4&);
	void rotate(float, const vec3&);
	void translate(const vec3&);

	void viewport(const vec4&);
	void clearColor(const vec4&);
	void clear();

	void begin();
	void end();
	void drawLine(const vec2&, const vec2&);
	void drawLine(const vec3&, const vec3&);
	void drawWireCube(const vec3&, const vec3&);
	void drawVector(const vec3&, const vec3&, float, float);

	mat4 getModelViewProjection();
}

#endif
