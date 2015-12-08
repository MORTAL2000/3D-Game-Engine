#include "MeshBuilder.h"

MeshBuilder& MeshBuilder::getInstance()
{
	static MeshBuilder instance;
	return instance;
}

void MeshBuilder::load(SceneNode* scene, const char* path)
{
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	vector<Material> newMaterials;
	vector<Triangle> tris;

	string err = tinyobj::LoadObj(shapes, materials, path);
	if(!err.empty())
	{
		printf("Error loading obj: %s", err.c_str());
	}

	for(size_t i = 0; i < materials.size(); i++)
	{
		vec3 color = vec3(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
		vec3 ambient = vec3(materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
		vec3 emission = vec3(materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
		newMaterials.push_back(Material(color, ambient, emission));
	}

	printf("Materials %d\n", (int)materials.size());
	printf("Shapes %d\n", (int)shapes.size());

	for(size_t i = 0; i < shapes.size(); i++)
	{
		size_t indices_size = shapes[i].mesh.indices.size() / 3;
		for(size_t j = 0; j < indices_size; j++)
		{
			vec3 v0 = vec3(shapes[i].mesh.positions[shapes[i].mesh.indices[3*j] * 3],
					shapes[i].mesh.positions[shapes[i].mesh.indices[3*j] * 3 + 1],
					shapes[i].mesh.positions[shapes[i].mesh.indices[3*j] * 3 + 2]);

			vec3 v1 = vec3(shapes[i].mesh.positions[shapes[i].mesh.indices[3*j+1] * 3],
					shapes[i].mesh.positions[shapes[i].mesh.indices[3*j+1] * 3 + 1],
					shapes[i].mesh.positions[shapes[i].mesh.indices[3*j+1] * 3 + 2]);

			vec3 v2 = vec3(shapes[i].mesh.positions[shapes[i].mesh.indices[3*j+2] * 3],
					shapes[i].mesh.positions[shapes[i].mesh.indices[3*j+2] * 3 + 1],
					shapes[i].mesh.positions[shapes[i].mesh.indices[3*j+2] * 3 + 2]);

			Triangle tr(v0, v1, v2);
			if(shapes[i].mesh.material_ids[j] < (int)newMaterials.size())
			{
				tr.mat = newMaterials[shapes[i].mesh.material_ids[j]];
			}
			else
			{
				tr.mat = Material(vec3(1.0), vec3(0.0), vec3(0.0));
			}
			tris.push_back(tr);
		}

		// TODO: Create new MeshRenderer for each triangle
		// TODO: Merge triangles into another class
		// TODO: Create physics for new class
		// TODO: Insert into SceneGraph
	}
}
