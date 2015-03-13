#include "BulletMeshBody.h"

BulletMeshBody::BulletMeshBody()
{

}

BulletMeshBody::~BulletMeshBody()
{
	delete m_mesh;
	m_mesh = 0;
}

void BulletMeshBody::build(const quat& rotation, const vec3& position)
{
	if(!m_renderer)
	{
		Console::log("Assign a mesh renderer first before construction");
		return;
	}

	m_mesh = new btTriangleMesh();
	std::vector<float> vertices = m_renderer->getVertices();
	for(size_t i = 0; i < vertices.size(); i += 9)
	{
		btVector3 v0(vertices[i], vertices[i+1], vertices[i+2]);
		btVector3 v1(vertices[i+3], vertices[i+4], vertices[i+5]);
		btVector3 v2(vertices[i+6], vertices[i+7], vertices[i+8]);
		m_mesh->addTriangle(v0, v1, v2);
	}

	m_shape = new btBvhTriangleMeshShape(m_mesh, true, true);
	m_shape->setMargin(0.04);
	BulletBody::construct(m_shape, rotation, position);
}
