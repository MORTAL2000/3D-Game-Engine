/*
 * Copyright 2015 Alexander Koch
 * File: Framebuffer.h
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

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <core/Commons.h>
#include <core/Console.h>

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void initialize(int, int);

	void bind();
	void unbind();
	void bindTexture(unsigned int, GLenum);
	void unbindTexture();

	void checkValidity();
private:
	GLuint m_framebuffer;
 	GLuint m_depthTexture;
	GLuint m_texture;
};

#endif
