/*
 * Copyright 2015 Alexander Koch
 * File: ObjLoader.h
 * Description: Wavefront OBJ loader.
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

#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
using std::vector;

#include <core/Commons.h>
#include <core/Console.h>
#include <core/FileReader.h>
#include <core/Tokenizer.h>

struct Index
{
	vector<int> indices;
	vector<int> texCoordIndex;
	vector<int> normalIndex;
};

struct Mtl
{
	string name, map_Kd_path;
	vec3 ka, kd, ks;

	Mtl(string _name = "")
	{
		kd = vec3(1.0);
		name = _name;
	}
};

struct Obj
{
	vector<float> vertices;
	vector<float> normals;
	vector<float> texcoords;
	Mtl mtl;
};

struct FaceGroup
{
	Mtl mtl;
	vector<Index> faces;

	FaceGroup(vector<Index>& _faces)
	{
		faces = _faces;
	}
};

class ObjLoader
{
public:
	static ObjLoader& getInstance();

	int loadSingleObject(const string& filename, int index, Obj& object);
	int load(const string& filename, vector<Obj>& objects);
	int loadMtl(const string& filename, vector<Mtl>& materials);
private:
	ObjLoader();
};

#endif
