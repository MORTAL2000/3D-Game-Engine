/*
 * Copyright 2015 Alexander Koch
 * File: MeshNode.cpp
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

#include "MeshNode.h"
#include <core/Context.h>

MeshNode::MeshNode()
{
	SceneNode::m_type = SceneNode::Type::MESH;
}

MeshNode::MeshNode(MeshRenderer* renderer)
{
	setRenderer(renderer);
	SceneNode::m_type = SceneNode::Type::MESH;
}

void MeshNode::draw(const mat4& parentTransform, FilmCamera* camera)
{
	if(m_renderer)
	{
		m_transformation = m_renderer->getModelMatrix();
		mat4 childTransform = parentTransform * m_transformation;

		// Get the new BoundingBox
		BoundingBox bbox = m_renderer->getBoundingBox();
		bbox.transform(childTransform);

		// Test if in view frustum
		if(camera->boxInFrustum(bbox))
		{
			Context::getInstance().addDrawCall();
			m_renderer->update(childTransform, camera);
			m_renderer->render();
		}

		for(NodePointerList::const_iterator ci = m_children.begin(); ci != m_children.end(); ci++)
		{
			(*ci)->draw(childTransform, camera);
		}
	}
}

void MeshNode::draw(const mat4& parentTransform, Shader* shader)
{
	if(m_renderer)
	{
		m_transformation = m_renderer->getModelMatrix();
		mat4 childTransform = parentTransform * m_transformation;

		// Standard info
		shader->bind();
		shader->mat4x4("modelMatrix", childTransform);
		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(childTransform)));
		shader->mat3x3("normalMatrix", normalMatrix);
		shader->unbind();

		// Draw call
		m_renderer->render(shader);

		for(NodePointerList::const_iterator ci = m_children.begin(); ci != m_children.end(); ci++)
		{
			(*ci)->draw(childTransform, shader);
		}
	}
}

void MeshNode::setRenderer(MeshRenderer* renderer)
{
	m_renderer = renderer;
}
