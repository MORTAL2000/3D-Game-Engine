#include "RenderableNode.h"

RenderableNode::RenderableNode() : m_selected(false)
{
	setName("Object0");
}

MeshRenderer* RenderableNode::getRenderer()
{
	return &m_renderer;
}

void RenderableNode::setSelected(bool selected)
{
	m_selected = selected;
}

bool RenderableNode::getIsSelected()
{
	return m_selected;
}

void RenderableNode::setType(const Type& type)
{
	m_type = type;
}

RenderableNode::Type RenderableNode::getType()
{
	return m_type;
}

void RenderableNode::setCollisionType(const Collision& collision)
{
	m_collision = collision;
}

RenderableNode::Collision RenderableNode::getCollisionType()
{
	return m_collision;
}

void RenderableNode::setColliderType(const Collider& collider)
{
	m_collider = collider;
}

RenderableNode::Collider RenderableNode::getColliderType()
{
	return m_collider;
}

void RenderableNode::setMeshPath(const std::string& path)
{
	m_path = path;
}

std::string RenderableNode::getMeshPath()
{
	return m_path;
}

void RenderableNode::setMass(double mass)
{
	m_physics.mass = mass;
}

void RenderableNode::setFriction(double friction)
{
	m_physics.friction = friction;
}

void RenderableNode::setRestitution(double restitution)
{
	m_physics.restitution = restitution;
}

void RenderableNode::setColliderScale(const vec3& scale)
{
	m_physics.scale = scale;
}

PhysicsInfo RenderableNode::getPhysicsInfo()
{
	return m_physics;
}

void RenderableNode::draw(const mat4& parentTransform, FilmCamera* camera)
{
	m_transformation = m_renderer.getModelMatrix();
	mat4 childTransform = parentTransform * m_transformation;

	m_renderer.update(childTransform, camera);
	m_renderer.render();

	for(NodePointerList::const_iterator ci = m_children.begin(); ci != m_children.end(); ci++)
	{
		(*ci)->draw(childTransform, camera);
	}
}

void RenderableNode::draw(const mat4& parentTransform, Shader* shader)
{
	m_transformation = m_renderer.getModelMatrix();
	mat4 childTransform = parentTransform * m_transformation;

	Shader* shdr = m_renderer.getMaterial()->getShader();
	shdr->bind();
	shdr->mat4x4("modelMatrix", childTransform);
	mat3 normalMatrix = glm::transpose(glm::inverse(mat3(childTransform)));
	shdr->mat3x3("normalMatrix", normalMatrix);
	shdr->unbind();

	m_renderer.render(shader);

	for(NodePointerList::const_iterator ci = m_children.begin(); ci != m_children.end(); ci++)
	{
		(*ci)->draw(childTransform, shader);
	}
}
