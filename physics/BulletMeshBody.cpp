/*
 * Copyright 2015 Alexander Koch
 * File: BulletMeshBody.cpp
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

#include "BulletMeshBody.h"

BulletMeshBody::BulletMeshBody() {}

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
