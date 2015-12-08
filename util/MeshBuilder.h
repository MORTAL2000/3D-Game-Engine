#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include <core/Commons.h>
#include <tinyobjloader/tiny_obj_loader.h>
#include <scene/SceneNode.h>

class MeshBuilder
{
public:
	static MeshBuilder& getInstance();
	void load(SceneNode* scene, const char* path);

private:
	struct Material
	{
		vec3 color;
		vec3 ambient;
		vec3 emission;

		Material(const vec3& c, const vec3& a, const vec3& e)
		{
			color = c;
			ambient = a;
			emission = e;
		}

		Material() {}
	};

	struct Triangle
	{
		vec3 v0;
		vec3 v1;
		vec3 v2;
		Material mat;

		Triangle(const vec3& v0, const vec3& v1, const vec3& v2)
		: v0(v0), v1(v1), v2(v2) {}
	};

	MeshBuilder() {}
};

#endif
