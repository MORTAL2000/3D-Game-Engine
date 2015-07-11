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

		// Get the BoundingBox
		BoundingBox bbox = m_renderer->getBoundingBox();
		vec3 min = bbox.getMin();
		vec3 max = bbox.getMax();

		vec4 min4 = childTransform * vec4(min.x, min.y, min.z, 1.0f);
		vec4 max4 = childTransform * vec4(max.x, max.y, max.z, 1.0f);

		min.x = min4.x;
		min.y = min4.y;
		min.z = min4.z;

		max.x = max4.x;
		max.y = max4.y;
		max.z = max4.z;
		bbox = BoundingBox(min, max);

		// Test if visible
		bool visible = camera->boxInFrustum(bbox);
		if(!visible)
		{
			return;
		}

		Context::getInstance().addDrawCall();

		m_renderer->update(childTransform, camera);
		m_renderer->render();

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

		Shader* shdr = m_renderer->getMaterial()->getShader();
		shdr->bind();
		shdr->mat4x4("modelMatrix", childTransform);
		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(childTransform)));
		shdr->mat3x3("normalMatrix", normalMatrix);
		shdr->unbind();

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
