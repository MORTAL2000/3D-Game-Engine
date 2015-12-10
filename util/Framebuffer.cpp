/*
 * Copyright 2015 Alexander Koch
 * File: Framebuffer.cpp
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

#include "Framebuffer.h"

Framebuffer::Framebuffer() : m_framebuffer(0), m_depthTexture(0), m_texture(0) {}

Framebuffer::~Framebuffer()
{
	glDeleteTextures(1, &m_texture);
	glDeleteTextures(1, &m_depthTexture);
	unbind();
	glDeleteFramebuffers(1, &m_framebuffer);
}

void Framebuffer::initialize(int width, int height)
{
	// create depth texture
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// create and bind colors texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// create framebuffer
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	// check and unbind
	checkValidity();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::bind()
{
	glEnable(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void Framebuffer::unbind()
{
	glDisable(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bindTexture(unsigned int texture, GLenum unit)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, (texture == 0) ? m_texture : m_depthTexture);
}

void Framebuffer::unbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void Framebuffer::checkValidity()
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		switch(status)
		{
			case GL_FRAMEBUFFER_UNSUPPORTED:
				Console::log("Unsupported framebuffer format");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				Console::log("Framebuffer incomplete, missing attachment");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				Console::log("Framebuffer incomplete, missing draw buffer");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				Console::log("Framebuffer incomplete, missing read buffer");
				break;
			default:
				Console::log("Unknown Framebuffer error");
				break;
		}
	}
}
