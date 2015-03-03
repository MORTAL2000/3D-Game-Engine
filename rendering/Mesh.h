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

#include "../core/Commons.h"
#include "../util/ObjLoader.h"
#include "../util/Shader.h"

class Mesh
{
public:
	Mesh();
	Mesh(const std::string&);
	~Mesh();

	void load(std::vector<vec3>);
	void load(std::vector<float>);
	void load(std::vector<vec3>, std::vector<vec3>, std::vector<vec2>);
	void load(std::vector<float>, std::vector<float>, std::vector<float>);
	int load(const std::string&);
	int loadSingleObject(const std::string&, int);

	void loadCube(float);
	void loadPlane(float);
	void loadIcosahedron(float);

	void render(Shader*);
	void render(Shader*, GLenum, float);

	int getVertexCount();
	int getNormalCount();
	int getTexCoordCount();

	std::vector<float> getVertices();
protected:
	//uint vao;
 	unsigned int buffer[3];
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texcoords;
	bool m_valid;

	void apply();
	void clear();
};

#endif
