/*
 * Copyright 2015 Alexander Koch
 * File: Shader.h
 * Description: A GLSL-Shader loader.
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

#ifndef SHADER_H
#define SHADER_H

#include <core/Commons.h>
#include <core/Console.h>
#include <core/FileReader.h>

#include <stdio.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

class Shader {
private:
	bool compiled;
	string vshFilename, fshFilename;

	unsigned int vsh;
	unsigned int fsh;
	unsigned int shader;

	void printLog(unsigned int);
	void include(string&, string);
	void destroy();

public:
	Shader();
	Shader(const string&, const string&, bool);
	~Shader();

	bool load(string, string);
	bool loadRaw(const char*, const char*);

	void bind();
	void unbind();
	bool isCompiled();

	// float
	void valuef(const char*, float);
	void vec2f(const char*, float, float);
	void vec3f(const char*, float, float, float);
	void vec4f(const char*, float, float, float, float);

	// int
	void valuei(const char*, int);
	void vec2i(const char*, int, int);
	void vec3i(const char*, int, int, int);
	void vec4i(const char*, int, int, int, int);

	// glm
	void vec2f(const char*, vec2);
	void vec3f(const char*, vec3);
	void vec4f(const char*, vec4);
	void mat3x3(const char*, mat3);
	void mat4x4(const char*, mat4);
	int getAttribute(const char*);
};

#endif
