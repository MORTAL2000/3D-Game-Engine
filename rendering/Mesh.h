/**
 * @file Mesh.h
 * @author Alexander Koch
 * @desc Triangle mesh
 */

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
using std::vector;
using std::string;

#include <core/Commons.h>
#include <util/ObjLoader.h>
#include <util/Shader.h>
#include <rendering/BoundingBox.h>

/**
 *	Mesh Object, stores vertices, normals and uv-coords
 */
class Mesh
{
public:
	Mesh();
	Mesh(const string& filename);
	~Mesh();

	void load(const vector<vec3>& vertices);
	void load(const vector<float>& vertices);
	void load(const vector<vec3>& vertices, const vector<vec3>& normals, const vector<vec2>& uvs);
	void load(const vector<float>& vertices, const vector<float>& normals, const vector<float>& uvs);
	int load(const string& filename);
	int loadSingleObject(const string& filename, int index);

	void loadCube(float);
	void loadCube(const vec3& position, const vec3& scale);
	void loadPlane(float);
	void loadIcosahedron(float);

	void render(Shader*);
	void render(Shader*, GLenum, float);

 	size_t getVertexCount();
	size_t getNormalCount();
	size_t getTexCoordCount();

	vector<float> getVertices();
	BoundingBox getBoundingBox();
protected:
	unsigned int vao;
 	unsigned int buffer[3];
	vector<float> m_vertices;
	vector<float> m_normals;
	vector<float> m_texcoords;
	vector<vec3> m_points;
	bool m_valid;
	BoundingBox m_bbox;

	void apply();
	void clear();
};

#endif
