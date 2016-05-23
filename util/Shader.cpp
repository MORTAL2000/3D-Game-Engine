/*
 * Copyright 2015 Alexander Koch
 * File: Shader.cpp
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

#include "Shader.h"

Shader::Shader() : compiled(false) {}

Shader::Shader(const string& vertex, const string& fragment, bool raw = false) : compiled(false) {
	if(!raw) {
		load(vertex, fragment);
	}
	else {
		loadRaw(vertex.c_str(), fragment.c_str());
	}
}

Shader::~Shader() {
	if(compiled) destroy();
}

// Private methods
void Shader::printLog(unsigned int shaderId) {
	int msg_len = 0;
	int out = 0;
	char* log_msg;

	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &msg_len);
	if(msg_len > 0) {
		log_msg = new char[msg_len];
		glGetShaderInfoLog(shaderId, msg_len, &out, log_msg);
		Console::log("Error: \n'%s'\n", log_msg);
		delete[] log_msg;
	}
	else {
		Console::log("Unknown error");
	}
}

void Shader::destroy() {
	glDetachShader(shader, fsh);
	glDetachShader(shader, vsh);

	glDeleteProgram(shader);
	glDeleteShader(fsh);
	glDeleteShader(vsh);
	compiled = false;
}

// Public methods

// loads a vertex and a fragment shader into the program
bool Shader::load(string vshFilename, string fshFilename) {
	this->vshFilename = vshFilename;
	this->fshFilename = fshFilename;
	string vshSource, fshSource;

	if(!FileReader::read(vshFilename, vshSource)) {
		Console::log("File is invalid [%s]", vshFilename.c_str());
		return false;
	}
	if(!FileReader::read(fshFilename, fshSource)) {
		Console::log("File is invalid [%s]", fshFilename.c_str());
		return false;
	}

	return loadRaw(vshSource.c_str(), fshSource.c_str());
}

bool Shader::loadRaw(const char* vertex_src, const char* fragment_src) {
	if(!glCreateShader) {
		Console::log("Shaders are not supported on your graphics card");
		return false;
	}

	if(compiled) destroy();
	vsh = glCreateShader(GL_VERTEX_SHADER);
	fsh = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsh, 1, &vertex_src, 0);
	glShaderSource(fsh, 1, &fragment_src, 0);

	// Compile the shaders
 	auto compile = 0;
	glCompileShader(vsh);
	glGetShaderiv(vsh, GL_COMPILE_STATUS, &compile);
	if(!compile) {
		if(!vshFilename.empty()) {
			Console::log("Compilation error [%s]", vshFilename.c_str());
		}

		printLog(vsh);
		glDeleteShader(vsh);
		return false;
	}

	glCompileShader(fsh);
	glGetShaderiv(fsh, GL_COMPILE_STATUS, &compile);
	if(!compile) {
		if(!fshFilename.empty()) {
			Console::log("Compilation error [%s]", fshFilename.c_str());
		}

		printLog(fsh);
		glDeleteShader(vsh);
		glDeleteShader(fsh);
		return false;
	}

	// Create program and link
	shader = glCreateProgram();
	glAttachShader(shader, fsh);
	glAttachShader(shader, vsh);
	glLinkProgram(shader);
	return (compiled = true);
}

void Shader::bind() {
	if(compiled) glUseProgram(shader);
}

void Shader::unbind() {
	glUseProgram(0);
}

bool Shader::isCompiled() {
	return compiled;
}

// uniform pass: float
void Shader::valuef(const char* var, float value) {
	glUniform1f(glGetUniformLocation(shader, var), value);
}

void Shader::vec2f(const char* var, float v1, float v2)
{
	glUniform2f(glGetUniformLocation(shader, var), v1, v2);
}

void Shader::vec3f(const char* var, float v1, float v2, float v3)
{
	glUniform3f(glGetUniformLocation(shader, var), v1, v2, v3);
}

void Shader::vec4f(const char* var, float v1, float v2, float v3, float v4)
{
	glUniform4f(glGetUniformLocation(shader, var), v1, v2, v3, v4);
}

// int
void Shader::valuei(const char* var, int value)
{
	glUniform1i(glGetUniformLocation(shader, var), value);
}

void Shader::vec2i(const char* var, int v1, int v2)
{
	glUniform2i(glGetUniformLocation(shader, var), v1, v2);
}

void Shader::vec3i(const char* var, int v1, int v2, int v3)
{
	glUniform3i(glGetUniformLocation(shader, var), v1, v2, v3);
}

void Shader::vec4i(const char* var, int v1, int v2, int v3, int v4)
{
	glUniform4i(glGetUniformLocation(shader, var), v1, v2, v3, v4);
}

// glm vector types
void Shader::vec2f(const char* var, vec2 vec)
{
	vec2f(var, vec.x, vec.y);
}

void Shader::vec3f(const char* var, vec3 vec)
{
	vec3f(var, vec.x, vec.y, vec.z);
}

void Shader::vec4f(const char* var, vec4 vec)
{
	vec4f(var, vec.x, vec.y, vec.z, vec.w);
}

void Shader::mat3x3(const char* var, mat3 matrix)
{
	glUniformMatrix3fv(glGetUniformLocation(shader, var), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::mat4x4(const char* var, mat4 matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(shader, var), 1, GL_FALSE, glm::value_ptr(matrix));
}

// Attribute location
int Shader::getAttribute(const char* attrib)
{
	return glGetAttribLocation(shader, attrib);
}
