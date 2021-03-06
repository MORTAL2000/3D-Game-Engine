/*
 * Copyright 2015 Alexander Koch
 * File: BulletMeshBody.h
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

#ifndef BULLET_MESH_BODY_H
#define BULLET_MESH_BODY_H

#include "BulletBody.h"

class BulletMeshBody : public BulletBody
{
public:
	BulletMeshBody();
	~BulletMeshBody();

	void build(const quat&, const vec3&);
private:
	btBvhTriangleMeshShape* m_shape;
	btTriangleMesh* m_mesh;
};

#endif
