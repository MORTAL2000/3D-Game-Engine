/*
 * Copyright 2015 Alexander Koch
 * File: MeshRenderer.cpp
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

#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() :
	m_material(0), m_position(0.0), m_scale(1.0),
	m_cullFace(GL_BACK), m_useDepthMask(true)
{}

void MeshRenderer::setMaterial(Material* material)
{
	m_material = material;
}

Material* MeshRenderer::getMaterial()
{
	return m_material;
}

void MeshRenderer::setPosition(const vec3& position)
{
	m_position = position;
}

void MeshRenderer::setRotation(const vec3& rotation)
{
 	// quat qx = quat(vec3(1.0f, 0.0f, 0.0f), rotation.x);
	// quat qy = quat(vec3(0.0f, 1.0f, 0.0f), rotation.y);
	// quat qz = quat(vec3(0.0f, 0.0f, 1.0), rotation.z);
	// m_rotation = qx * qy * qz;
	m_rotation = quat(rotation);
}

void MeshRenderer::setRotation(const quat& rotation)
{
	m_rotation = rotation;
}

void MeshRenderer::setScale(const vec3& scale)
{
	m_scale = scale;
}

vec3 MeshRenderer::getPosition() const
{
	return m_position;
}

quat MeshRenderer::getRotation() const
{
	return m_rotation;
}

vec3 MeshRenderer::getScale() const
{
	return m_scale;
}

void MeshRenderer::setCullFace(GLenum face)
{
	m_cullFace = face;
}

void MeshRenderer::setUseDepthMask(bool state)
{
	m_useDepthMask = state;
}

GLenum MeshRenderer::getCullFace()
{
	return m_cullFace;
}

bool MeshRenderer::getDepthMaskUsage()
{
	return m_useDepthMask;
}

mat4 MeshRenderer::getModelMatrix()
{
	mat4 translationMatrix = glm::translate(mat4(1.0), m_position);
	mat4 rotationMatrix = glm::mat4_cast(m_rotation);
	mat4 scaleMatrix = glm::scale(mat4(1.0), m_scale);
	//	mat4 modelMatrix = rotationMatrix * scaleMatrix * translationMatrix;
	return translationMatrix * rotationMatrix * scaleMatrix;
}

void MeshRenderer::update(FilmCamera* camera)
{
	m_material->update(camera, getModelMatrix());
}

void MeshRenderer::update(const mat4& transformation, FilmCamera* camera)
{
	m_material->update(camera, transformation);
}

void MeshRenderer::render(Shader* shader)
{
	if(m_cullFace == GL_NONE) {
		glDisable(GL_CULL_FACE);
	} else {
		glCullFace(m_cullFace);
	}

	glDepthMask(m_useDepthMask);
	m_material->bindTextures();
	Mesh::render(shader);
	m_material->unbindTextures();
	glDepthMask(GL_TRUE);

	if(m_cullFace == GL_NONE) {
		glEnable(GL_CULL_FACE);
	} else {
		glCullFace(GL_BACK);
	}
}

void MeshRenderer::render()
{
	render(m_material->getShader());
}

int MeshRenderer::getTriangleCount()
{
	return int(getVertexCount()/3);
}
