/**
 * @file Shader.h
 * @author Alexander Koch
 * @desc A GLSL-Shader loader
 */

#ifndef SHADER_H
#define SHADER_H

#include "../core/Commons.h"

#include <stdio.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

#include "../core/Console.h"
#include "../core/FileReader.h"

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

	/* Use */
	bool load(string, string);
	bool loadRaw(string, string);

	void bind();
	void unbind();
	bool isCompiled();

	/* float */
	void valuef(const char*, float);
	void vec2f(const char*, float, float);
	void vec3f(const char*, float, float, float);
	void vec4f(const char*, float, float, float, float);

	/* int */
	void valuei(const char*, int);
	void vec2i(const char*, int, int);
	void vec3i(const char*, int, int, int);
	void vec4i(const char*, int, int, int, int);

	/* glm */
	void vec2f(const char*, vec2);
	void vec3f(const char*, vec3);
	void vec4f(const char*, vec4);
	void mat3x3(const char*, mat3);
	void mat4x4(const char*, mat4);

	/* get shader attributes */
	int getAttribute(const char*);
};

#endif
