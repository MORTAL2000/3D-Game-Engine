#include "Mesh.h"

Mesh::Mesh() :
	m_valid(false)
{
}

Mesh::Mesh(const std::string& filename) :
	m_valid(false)
{
	load(filename);
}

Mesh::~Mesh()
{

	clear();
}

void Mesh::load(std::vector<vec3> vertices)
{
	vector<float> _vertices, _normals, _texcoords;
	for(auto i = 0; i < vertices.size(); i++)
	for(auto j = 0; j < 3; j++)
	_vertices.push_back(vertices[i][j]);

	load(_vertices, _normals, _texcoords);
}

void Mesh::load(std::vector<float> vertices)
{
	std::vector<float> empty;
	load(vertices, empty, empty);
}

void Mesh::load(vector<vec3> vertices, vector<vec3> normals, vector<vec2> texcoords)
{
	vector<float> _vertices, _normals, _texcoords;
	size_t i, j;
	for(i = 0; i < vertices.size(); i++)
	for(j = 0; j < 3; j++)
	_vertices.push_back(vertices[i][j]);

	for(i = 0; i < normals.size(); i++)
	for(j = 0; j < 3; j++)
	_normals.push_back(normals[i][j]);

	for(i = 0; i < texcoords.size(); i++)
	for(j = 0; j < 2; j++)
	_texcoords.push_back(texcoords[i][j]);

	load(_vertices, _normals, _texcoords);
}

void Mesh::load(vector<float> vertices, vector<float> normals, vector<float> texcoords)
{
	this->vertices = vertices;
	this->normals = normals;
	this->texcoords = texcoords;

	if(!m_valid)
	{
		glGenBuffers(3, buffer);
		//glGenVertexArrays(1, &vao);
		m_valid = true;
	}
	apply();
}

int Mesh::load(const string& filename)
{
	vector<Obj> objects;
	if(!ObjLoader::getInstance().load(filename, objects)) return 0;

	vector<float> _vertices, _normals, _texcoords;
	for(auto i = 0, j = 0; i < objects.size(); i++)
	{
		for(j = 0; j < objects[i].vertices.size(); j++) _vertices.push_back(objects[i].vertices[j]);
		for(j = 0; j < objects[i].normals.size(); j++) _normals.push_back(objects[i].normals[j]);
		for(j = 0; j < objects[i].texcoords.size(); j++) _texcoords.push_back(objects[i].texcoords[j]);
	}

	load(_vertices, _normals, _texcoords);
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
	float _vertices[] =
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

	float _normals[] =
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

	float _uvs[] =
	{
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0,0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0,
		0,0, 1,0, 1,1, 1,1, 0,1, 0,0, 0,0, 1,0, 1,1, 1,1, 0,1, 0,0
	};

	for(auto i = 0; i < sizeof(_vertices)/sizeof(float); i++) vertices.push_back(_vertices[i] * size);
	for(auto j = 0; j < sizeof(_normals)/sizeof(float); j++) normals.push_back(_normals[j]);
	for(auto k = 0; k < sizeof(_uvs)/sizeof(float); k++) texcoords.push_back(_uvs[k]);

	load(vertices, normals, texcoords);
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
	for(auto i = 0; i < 6; i++)
	{
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

	std::vector<vec2> empty;
	load(_vertices, _normals, empty);
}

void Mesh::render(Shader* shader)
{
	render(shader, GL_TRIANGLES, vertices.size()/3);
}

void Mesh::render(Shader* shader, GLenum type, float size)
{
	//glBindVertexArray(vao);
	shader->bind();

	uint8_t vertex = 0;
	uint8_t normal = 1;
	uint8_t uv = 2;

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if(normals.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
		glEnableVertexAttribArray(normal);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if(texcoords.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
		glEnableVertexAttribArray(uv);
		glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glDrawArrays(type, 0, size);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vertex);
	if(normals.size() > 0) glDisableVertexAttribArray(normal);
	if(texcoords.size() > 0) glDisableVertexAttribArray(uv);

	shader->unbind();
	//glBindVertexArray(0);
}

int Mesh::getVertexCount()
{
	return int(vertices.size());
}

int Mesh::getNormalCount()
{
	return int(normals.size());
}

int Mesh::getTexCoordCount()
{
	return int(texcoords.size());
}

std::vector<float> Mesh::getVertices()
{
	return vertices;
}

/* Private */
void Mesh::apply()
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	if(normals.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
	}

	if(texcoords.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
		glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(float), texcoords.data(), GL_STATIC_DRAW);
	}
}

void Mesh::clear() {
	if(vertices.size() > 0)
	{
		//glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(3, buffer);
		vertices.clear();
		normals.clear();
		texcoords.clear();
	}
}
