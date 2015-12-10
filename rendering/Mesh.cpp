#include "Mesh.h"

#define MESH_VERTEX 0
#define MESH_NORMAL 1
#define MESH_UV 2

Mesh::Mesh() : m_valid(false) {}

Mesh::Mesh(const string& filename) : m_valid(false)
{
	load(filename);
}

Mesh::~Mesh()
{
	clear();
}

void Mesh::load(const vector<vec3>& vertices)
{
	vector<float> verts, norms, tex;
	for(auto i = 0; i < vertices.size(); i++)
	{
		for(auto j = 0; j < 3; j++)
		verts.push_back(vertices[i][j]);
	}

	load(verts, norms, tex);
}

void Mesh::load(const vector<float>& vertices)
{
	vector<float> empty;
	load(vertices, empty, empty);
}

void Mesh::load(const vector<vec3>& vertices, const vector<vec3>& normals, const vector<vec2>& uvs)
{
	vector<float> verts, norms, tex;
	size_t i, j;
	for(i = 0; i < vertices.size(); i++)
	{
		for(j = 0; j < 3; j++)
		verts.push_back(vertices[i][j]);
	}

	for(i = 0; i < normals.size(); i++)
	{
		for(j = 0; j < 3; j++)
		norms.push_back(normals[i][j]);
	}

	for(i = 0; i < uvs.size(); i++)
	{
		for(j = 0; j < 2; j++)
		tex.push_back(uvs[i][j]);
	}

	load(verts, norms, tex);
}

void Mesh::load(const vector<float>& vertices, const vector<float>& normals, const vector<float>& uvs)
{
	m_vertices = vertices;
	m_normals = normals;
	m_texcoords = uvs;

	if(!m_valid)
	{
		glGenBuffers(3, buffer);
		glGenVertexArrays(1, &vao);
		m_valid = true;
	}

	apply();
}

int Mesh::load(const string& filename)
{
	vector<Obj> objects;
	if(!ObjLoader::getInstance().load(filename, objects)) return 0;

	vector<float> verts, norms, tex;
	for(auto i = 0, j = 0; i < objects.size(); i++)
	{
		for(j = 0; j < objects[i].vertices.size(); j++) verts.push_back(objects[i].vertices[j]);
		for(j = 0; j < objects[i].normals.size(); j++) norms.push_back(objects[i].normals[j]);
		for(j = 0; j < objects[i].texcoords.size(); j++) tex.push_back(objects[i].texcoords[j]);
	}

	load(verts, norms, tex);
	return 1;
}

int Mesh::loadSingleObject(const string& filename, int index)
{
	Obj object;
	if(!ObjLoader::getInstance().loadSingleObject(filename.c_str(), index, object)) return 0;

	/*material.ambient = object.mtl.ka;
	material.color = vec4(object.mtl.kd, 1);
	material.specular = object.mtl.ks;

	if(object.mtl.map_Kd_path != string()) {
		material.texture.load(object.mtl.map_Kd_path);
		material.hasTexture = true;
		material.loaded = true;
	}*/

	load(object.vertices, object.normals, object.texcoords);
	return 1;
}

void Mesh::loadCube(float size)
{
	float verts[] =
	{
		1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (front)
		-1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0
		1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (right)
		1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0
		1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (top)
		-1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0
		-1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (left)
		-1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1
		-1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (bottom)
		1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7
		1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (back)
		-1, 1,-1,   1, 1,-1,   1,-1,-1      // v6-v5-v4
	};

	float norms[] =
	{
		0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (front)
		0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0
		1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
		1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0
		0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
		0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0
		-1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (left)
		-1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1
		0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (bottom)
		0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7
		0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (back)
		0, 0,-1,   0, 0,-1,   0, 0,-1       // v6-v5-v4
	};

	float uvs[] =
	{
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0,0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0
	};

	m_vertices.clear();
	m_normals.clear();
	m_texcoords.clear();

	for(auto i = 0; i < sizeof(verts)/sizeof(float); i++) m_vertices.push_back(verts[i] * size);
	for(auto j = 0; j < sizeof(norms)/sizeof(float); j++) m_normals.push_back(norms[j]);
	for(auto k = 0; k < sizeof(uvs)/sizeof(float); k++) m_texcoords.push_back(uvs[k]);

	load(m_vertices, m_normals, m_texcoords);
}

void Mesh::loadCube(const vec3& position, const vec3& scale)
{
	float verts[] =
	{
		1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (front)
		-1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0
		1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (right)
		1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0
		1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (top)
		-1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0
		-1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (left)
		-1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1
		-1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (bottom)
		1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7
		1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (back)
		-1, 1,-1,   1, 1,-1,   1,-1,-1      // v6-v5-v4
	};

	float norms[] =
	{
		0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (front)
		0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0
		1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
		1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0
		0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
		0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0
		-1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (left)
		-1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1
		0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (bottom)
		0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7
		0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (back)
		0, 0,-1,   0, 0,-1,   0, 0,-1       // v6-v5-v4
	};

	float uvs[] =
	{
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0,0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0
	};

	for(auto i = 0; i < sizeof(verts)/sizeof(float)/3; i++)
	{
		verts[i*3] = (verts[i*3] + position.x) * scale.x;
		verts[i*3+1] = (verts[i*3+1] + position.y) * scale.y;
		verts[i*3+2] = (verts[i*3+2] + position.z) * scale.z;
		m_vertices.push_back(verts[i*3]);
		m_vertices.push_back(verts[i*3+1]);
		m_vertices.push_back(verts[i*3+2]);
	}

	for(auto j = 0; j < sizeof(norms)/sizeof(float); j++) m_normals.push_back(norms[j]);
	for(auto k = 0; k < sizeof(uvs)/sizeof(float); k++) m_texcoords.push_back(uvs[k]);

	load(m_vertices, m_normals, m_texcoords);
}

void Mesh::loadPlane(float size)
{
	vector<vec3> _vertices, _normals;
	vector<vec2> _texcoords;

	float x = -1;
	float y = -1;
	float cx = 0;
	float cy = 0;
	float cxs = 1;
	float cys = 1;

	vec2 uv0 = vec2(cx, cy);
	vec2 uv1 = vec2(cxs, cy);
	vec2 uv2 = vec2(cxs, cys);
	vec2 uv3 = vec2(cx, cys);

	vec3 v0 = vec3(x * size, 0, y * size);
	vec3 v1 = vec3((x+2) * size, 0, y * size);
	vec3 v2 = vec3((x+2) * size, 0, (y+2) * size);
	vec3 v3 = vec3(x * size, 0, (y+2) * size);

	_vertices.push_back(v2);
	_vertices.push_back(v1);
	_vertices.push_back(v0);
	_vertices.push_back(v3);
	_vertices.push_back(v2);
	_vertices.push_back(v0);

	vec3 up = vec3(0, 1, 0);
	for(auto i = 0; i < 6; i++) {
		_normals.push_back(up);
	}

	_texcoords.push_back(uv2);
	_texcoords.push_back(uv1);
	_texcoords.push_back(uv0);
	_texcoords.push_back(uv3);
	_texcoords.push_back(uv2);
	_texcoords.push_back(uv0);

	load(_vertices, _normals, _texcoords);
}

void Mesh::loadIcosahedron(float radius)
{
	vector<vec3> _vertices;
	vector<vec3> _normals;

	float t = (1.0 + sqrt(5.0)) * 0.5;
	vec3 v0 = vec3(-1, t, 0);
	vec3 v1 = vec3(1, t, 0);
	vec3 v2 = vec3(-1, -t, 0);
	vec3 v3 = vec3(1, -t, 0);
	vec3 v4 = vec3(0, -1, t);
	vec3 v5 = vec3(0, 1, t);
	vec3 v6 = vec3(0, -1, -t);
	vec3 v7 = vec3(0, 1, -t);
	vec3 v8 = vec3(t, 0, -1);
	vec3 v9 = vec3(t, 0, 1);
	vec3 v10 = vec3(-t, 0, -1);
	vec3 v11 = vec3(-t, 0, 1);

	_vertices.push_back(v0);
	_vertices.push_back(v11);
	_vertices.push_back(v5);
	_vertices.push_back(v0);
	_vertices.push_back(v5);
	_vertices.push_back(v1);
	_vertices.push_back(v0);
	_vertices.push_back(v1);
	_vertices.push_back(v7);
	_vertices.push_back(v0);
	_vertices.push_back(v7);
	_vertices.push_back(v10);
	_vertices.push_back(v0);
	_vertices.push_back(v10);
	_vertices.push_back(v11);

	_vertices.push_back(v1);
	_vertices.push_back(v5);
	_vertices.push_back(v9);
	_vertices.push_back(v5);
	_vertices.push_back(v11);
	_vertices.push_back(v4);
	_vertices.push_back(v11);
	_vertices.push_back(v10);
	_vertices.push_back(v2);
	_vertices.push_back(v10);
	_vertices.push_back(v7);
	_vertices.push_back(v6);
	_vertices.push_back(v7);
	_vertices.push_back(v1);
	_vertices.push_back(v8);

	_vertices.push_back(v3);
	_vertices.push_back(v9);
	_vertices.push_back(v4);
	_vertices.push_back(v3);
	_vertices.push_back(v4);
	_vertices.push_back(v2);
	_vertices.push_back(v3);
	_vertices.push_back(v2);
	_vertices.push_back(v6);
	_vertices.push_back(v3);
	_vertices.push_back(v6);
	_vertices.push_back(v8);
	_vertices.push_back(v3);
	_vertices.push_back(v8);
	_vertices.push_back(v9);

	_vertices.push_back(v4);
	_vertices.push_back(v9);
	_vertices.push_back(v5);
	_vertices.push_back(v2);
	_vertices.push_back(v4);
	_vertices.push_back(v11);
	_vertices.push_back(v6);
	_vertices.push_back(v2);
	_vertices.push_back(v10);
	_vertices.push_back(v8);
	_vertices.push_back(v6);
	_vertices.push_back(v7);
	_vertices.push_back(v9);
	_vertices.push_back(v8);
	_vertices.push_back(v1);

	for(auto i = 0; i < _vertices.size(); i++)
	{
		_vertices[i] *= radius;
		_normals.push_back(normalize(_vertices[i] - vec3(0.0)));
	}

	vector<vec2> empty;
	load(_vertices, _normals, empty);
}

void Mesh::render(Shader* shader)
{
	render(shader, GL_TRIANGLES, m_vertices.size()/3);
}

void Mesh::render(Shader* shader, GLenum type, float size)
{
	glBindVertexArray(vao);
	shader->bind();

	glDrawArrays(type, 0, size);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader->unbind();
	glBindVertexArray(0);
}

size_t Mesh::getVertexCount()
{
	return m_vertices.size();
}

size_t Mesh::getNormalCount()
{
	return m_normals.size();
}

size_t Mesh::getTexCoordCount()
{
	return m_texcoords.size();
}

vector<float> Mesh::getVertices()
{
	return m_vertices;
}

// Private methods
void Mesh::apply()
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

	if(m_normals.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
		glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(float), m_normals.data(), GL_STATIC_DRAW);
	}

	if(m_texcoords.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
		glBufferData(GL_ARRAY_BUFFER, m_texcoords.size() * sizeof(float), m_texcoords.data(), GL_STATIC_DRAW);
	}

	// Store 'points' and rebuild bounding box
	vec3 first = vec3(m_vertices[0], m_vertices[1], m_vertices[2]);

	vec3 min = first, max = first;
	m_points.clear();
	for(int i = 0; i < m_vertices.size(); i+=3)
	{
		vec3 v = vec3(m_vertices[i], m_vertices[i+1], m_vertices[i+2]);
		m_points.push_back(v);

		if(v.x > max.x) max.x = v.x;
		if(v.y > max.y) max.y = v.y;
		if(v.z > max.z) max.z = v.z;

		if(v.x < min.x) min.x = v.x;
		if(v.y < min.y) min.y = v.y;
		if(v.z < min.z) min.z = v.z;
	}

	m_bbox.set(min, max);

	// Setup vertex array object
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glEnableVertexAttribArray(MESH_VERTEX);
	glVertexAttribPointer(MESH_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if(m_normals.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
		glEnableVertexAttribArray(MESH_NORMAL);
		glVertexAttribPointer(MESH_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if(m_texcoords.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
		glEnableVertexAttribArray(MESH_UV);
		glVertexAttribPointer(MESH_UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindVertexArray(0);
}

BoundingBox Mesh::getBoundingBox()
{
	return m_bbox;
}

void Mesh::clear() {
	if(m_vertices.size() > 0)
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(3, buffer);
		m_vertices.clear();
		m_normals.clear();
		m_texcoords.clear();
	}
}
