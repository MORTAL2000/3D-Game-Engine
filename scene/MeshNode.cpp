#include "MeshNode.h"

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
