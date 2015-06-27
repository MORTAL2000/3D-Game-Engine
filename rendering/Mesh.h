/**
 * @file Mesh.h
 * @author Alexander Koch
 * @desc triangle mesh
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

/**
 *	Mesh Object, stores vertices, normals and uv-coords
 */
class Mesh
{
public:
	Mesh();
	Mesh(const string&);
	~Mesh();

	void load(vector<vec3>);
	void load(vector<float>);
	void load(vector<vec3>, vector<vec3>, vector<vec2>);
	void load(vector<float>, vector<float>, vector<float>);
	int load(const string&);
	int loadSingleObject(const string&, int);

	void loadCube(float);
	void loadCube(const vec3& position, const vec3& scale);
	void loadPlane(float);
	void loadIcosahedron(float);

	void render(Shader*);
	void render(Shader*, GLenum, float);

	int getVertexCount();
	int getNormalCount();
	int getTexCoordCount();

	vector<float> getVertices();
protected:
	//uint vao;
 	unsigned int buffer[3];
	vector<float> vertices;
	vector<float> normals;
	vector<float> texcoords;
	bool m_valid;

	void apply();
	void clear();
};

#endif
