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
