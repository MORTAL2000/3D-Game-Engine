/*
 * Copyright 2015 Alexander Koch
 * File: ObjLoader.cpp
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

#include "ObjLoader.h"

ObjLoader::ObjLoader() {}

ObjLoader& ObjLoader::getInstance() {
	static ObjLoader instance;
	return instance;
}

int ObjLoader::loadSingleObject(const string& filename, int index, Obj& object) {
	vector<Obj> objects;
	if(!load(filename, objects)) return 0;
	if(index > objects.size()-1 || index < 0) return 0;
	object = objects[index];
	return 1;
}

int ObjLoader::load(const string& filename, vector<Obj>& objects) {
	if(Tokenizer::getFileExtension(filename) != "obj") {
		Console::log("Requested file is invalid or not and OBJ file : %s", filename.c_str());
		return 0;
	}

	vector<string> lines = FileReader::readLines(filename);
	if(lines.empty()) {
		Console::log("Error loading OBJ from : %s", filename.c_str());
		return 0;
	}

	vector<FaceGroup> groups;
	int o = 0, g = 0;
	vector<vec3> positions;
	vector<vec2> texCoords;
	vector<vec3> normals;
	vector<Index> faces;
	bool hasTexCoords = false, hasNormals = false;

	// external materials
	vector<Mtl> materials;
	size_t current_mtl_index = 0;

	for(auto i = 0; i < lines.size(); i++)
	{
		vector<string> tokens = Tokenizer::tokenize(lines[i], ' ');
		if(tokens.size() == 0 || tokens[0] == "#") continue;

		if(tokens[0] == "o")
		{
			if(++o > 1)
			{
				FaceGroup group(faces);

				if(materials.size() > 0)
				group.mtl = materials[current_mtl_index];

				groups.push_back(group);
				faces.clear();
			}
		}
		else if(tokens[0] == "g")
		{
			if(++g > 1)
			{
				FaceGroup group(faces);

				if(materials.size() > 0)
				group.mtl = materials[current_mtl_index];

				groups.push_back(group);
				faces.clear();
			}
		}
		else if(tokens[0] == "v")
		{
			vec3 v;
			for(uint8_t c = 0; c < 3; c++)
			sscanf(tokens[c+1].c_str(), "%f", &v[c]);
			positions.push_back(v);
		}
		else if(tokens[0] == "vt")
		{
			vec2 uv;
			for(uint8_t c = 0; c < 2; c++)
			sscanf(tokens[c+1].c_str(), "%f", &uv[c]);
			texCoords.push_back(uv);
		}
		else if(tokens[0] == "vn")
		{
			vec3 vn;
			for(uint8_t c = 0; c < 3; c++)
			sscanf(tokens[c+1].c_str(), "%f", &vn[c]);
			normals.push_back(vn);
		}
		else if(tokens[0] == "f")
		{
			Index index;

			for(size_t j = 1; j < tokens.size(); j++)
			{
				vector<string> sub_tokens = Tokenizer::tokenize(tokens[j], '/');
				if(!sub_tokens.size() || !sub_tokens[0].size() || sub_tokens[0].empty()) continue;

				int value0 = 0;
				sscanf(sub_tokens[0].c_str(), "%i", &value0);
				index.indices.push_back(value0-1);

				if(sub_tokens.size() > 1)
				{
					if(sub_tokens[1].size() && !sub_tokens[1].empty())
					{
						hasTexCoords = true;
						int value1 = 0;
						sscanf(sub_tokens[1].c_str(), "%i", &value1);
						index.texCoordIndex.push_back(value1-1);
					}
				}

				if(sub_tokens.size() > 2)
				{
					if(sub_tokens[2].size() && !sub_tokens[2].empty())
					{
						hasNormals = true;
						int value2 = 0;
						sscanf(sub_tokens[2].c_str(), "%i", &value2);
						index.normalIndex.push_back(value2-1);
					}
				}
			}

			// triangulation
			if(index.indices.size() == 4)
			{
				// v=vertex, n=normal, t=texture coordinate
				// pop back to erase last entry
				int v0 = index.indices[0];
				int v2 = index.indices[2];
				int v3 = index.indices[3];
				index.indices.pop_back();

				int n0, n2, n3, t0, t2, t3;
				if(hasNormals)
				{
					n0 = index.normalIndex[0];
					n2 = index.normalIndex[2];
					n3 = index.normalIndex[3];
					index.normalIndex.pop_back();
				}
				if(hasTexCoords)
				{
					t0 = index.texCoordIndex[0];
					t2 = index.texCoordIndex[2];
					t3 = index.texCoordIndex[3];
					index.texCoordIndex.pop_back();
				}
				faces.push_back(index);
				// upload index / one triangle
				// create second triangle
				// Vertices
				index.indices.clear();
				index.indices.push_back(v2);
				index.indices.push_back(v3);
				index.indices.push_back(v0);

				// Normals
				if(hasNormals)
				{
					index.normalIndex.clear();
					index.normalIndex.push_back(n2);
					index.normalIndex.push_back(n3);
					index.normalIndex.push_back(n0);
				}

				/* Coordinates */
				if(hasTexCoords)
				{
					index.texCoordIndex.clear();
					index.texCoordIndex.push_back(t2);
					index.texCoordIndex.push_back(t3);
					index.texCoordIndex.push_back(t0);
				}

				faces.push_back(index);
			}
			else if(index.indices.size() == 3) faces.push_back(index);
			else
			{
				Console::log("Face vertices are not convertable");
				Console::log("Size : %d (line : %d)", index.indices.size(), i);
				return 0;
			}
		}
		else if(tokens[0] == "mtllib")
		{
			string directory = Tokenizer::getDirectory(filename, true);
			string path = directory+tokens[1];

			vector<Mtl> sub_materials;
			if(!loadMtl(path, sub_materials))
			{
				Console::log("Failed at loading material");
				return 0;
			}
			for(size_t t = 0; t < sub_materials.size(); t++) materials.push_back(sub_materials[t]);
		}
		else if(tokens[0] == "usemtl")
		{
			for(size_t t = 0; t < materials.size(); t++)
			if(materials[t].name == tokens[1])
			{
				current_mtl_index = t;
				break;
			}
		}
	}

	if(positions.size() > 0)
	{
		FaceGroup group(faces);

		if(materials.size() > 0)
		group.mtl = materials[current_mtl_index];
		groups.push_back(group);
	}

	/********************************************************/
	/* An index contains three or four vertices 			*/
	/* if the mesh is smooth shaded (has vertex normals),   */
	/* an index contains indices with given normals         */
	/* e.g. : f 45/34/23 34/56/4 34/948/23					*/
	/* converts to :										*/
	/* vertex[45], normalIndex[34], uvIndex[23]				*/
	/* vertex[34], normalIndex[56], uvIndex[4]				*/
	/* [...]												*/
	/********************************************************/

	for(size_t o = 0; o < groups.size(); o++)
	{
		vector<float> finalVertices;
		vector<float> finalNormals;
		vector<float> finalUvs;

		// for each object
		// for each face
		// for each vertex in face[i]
		// get the vertex position, substracted by one

		// if we have vertex normals or uvs, store them

		for(size_t i = 0; i < int(groups[o].faces.size()); i++)
		{
			for(size_t j = 0; j < groups[o].faces[i].indices.size(); j++)
			{
				unsigned int v = groups[o].faces[i].indices[j];
				if(v > positions.size()-1)
				{
					Console::log("Vertex id out of bounds : %d", v);
					return 0;
				}
				vec3 vertex = positions[v];

				if(hasNormals)
				{
					unsigned int n = groups[o].faces[i].normalIndex[j];
					if(n > normals.size()-1)
					{
						Console::log("Normal id out of bounds : %d", n);
						return 0;
					}
					vec3 normal = normals[n];
					for(uint8_t c = 0; c < 3; c++) finalNormals.push_back(normal[c]);
				}

				if(hasTexCoords)
				{
					unsigned int t = groups[o].faces[i].texCoordIndex[j];
					if(t > texCoords.size()-1)
					{
						Console::log("Texture coordinate id out of bounds : %d", t);
						return 0;
					}
					vec2 uv = texCoords[t];
					for(uint8_t c = 0; c < 2; c++) finalUvs.push_back(uv[c]);
				}

				for(uint8_t k = 0; k < 3; k++)
				{
					finalVertices.push_back(vertex[k]);
				}
			}
		}

		Obj object;
		object.vertices = finalVertices;
		object.normals = finalNormals;
		object.texcoords = finalUvs;
		object.mtl = groups[o].mtl;
		objects.push_back(object);
	}
	groups.clear();
	return 1;
}

int ObjLoader::loadMtl(const string& filename, vector<Mtl>& materials) {
	vector<string> lines = FileReader::readLines(filename);
	if(lines.empty()) {
		Console::log("Error loading MTL from %s", filename.c_str());
		return 0;
	}

	Mtl* current_mtl = 0;
	for(size_t i = 0; i < lines.size(); i++) {
		vector<string> tokens = Tokenizer::tokenize(lines[i], ' ');
		if(tokens.size() == 0 || tokens[0] == "#") continue;

		if(tokens[0] == "newmtl")
		{
			Mtl newMtl(tokens[1]);
			materials.push_back(newMtl);
			current_mtl = &materials[materials.size()-1];
		}
		else if(tokens[0] == "Ka")
		{
			if(tokens.size() < 4) {
				Console::log("Ambient color contains less than three values (line : %d)", i);
				materials.clear();
				return 0;
			}
			if(current_mtl == 0) return 0;

			vec3 ka = vec3(0.0);
			for(int i = 0; i < 3; i++)
			sscanf(tokens[i+1].c_str(), "%f", &ka[i]);
			current_mtl->ka = ka;
		}
		else if(tokens[0] == "Kd")
		{
			if(tokens.size() < 4)
			{
				Console::log("Diffuse color contains less than three values (line : %d)", i);
				materials.clear();
				return 0;
			}
			if(current_mtl == 0) return 0;

			vec3 kd = vec3(0.0);
			for(int i = 0; i < 3; i++)
			sscanf(tokens[i+1].c_str(), "%f", &kd[i]);
			current_mtl->kd = kd;
		}
		else if(tokens[0] == "Ks")
		{
			if(tokens.size() < 4)
			{
				Console::log("Specular color contains less than three values (line : %d)", i);
				materials.clear();
				return 0;
			}
			if(current_mtl == 0) return 0;

			vec3 ks = vec3(0.0);
			for(int i = 0; i < 3; i++)
			sscanf(tokens[i+1].c_str(), "%f", &ks[i]);
			current_mtl->ks = ks;
		}
		else if(tokens[0] == "map_Kd")
		{
			if(current_mtl == 0) return 0;
			string dir = Tokenizer::getDirectory(filename, true);
			string path = dir + tokens[1];
			current_mtl->map_Kd_path = path;
		}
	}

	return 1;
}
