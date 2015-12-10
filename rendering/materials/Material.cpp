/*
 * Copyright 2015 Alexander Koch
 * File: Material.cpp
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

#include "Material.h"

Material::Material() : m_shader(new Shader())
{}

Material::~Material()
{}

void Material::load()
{}

void Material::update(FilmCamera* camera, const mat4& modelMatrix)
{
	m_shader->bind();
	m_shader->mat4x4("projectionMatrix", camera->getProjectionMatrix());
	m_shader->mat4x4("viewMatrix", camera->getViewMatrix());
	m_shader->mat4x4("modelMatrix", modelMatrix);
	m_shader->vec3f("cameraPosition", camera->getPosition());

	mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelMatrix)));
	m_shader->mat3x3("normalMatrix", normalMatrix);
	m_shader->unbind();
}

Shader* Material::getShader()
{
	return m_shader.get();
}

void Material::bindTextures()
{
	for(auto it = m_texture_cache.begin(); it != m_texture_cache.end(); it++)
	{
		ITexture* texture = it->second;
		if(texture)
		{
			long id = texture->getId();
			glActiveTexture(GL_TEXTURE0 + id);
			texture->bind(GL_TEXTURE0 + id);
		}
	}
}

void Material::unbindTextures()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);
}
