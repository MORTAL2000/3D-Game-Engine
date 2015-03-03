/**
 * @file ObjLoader.h
 * @author Alexander Koch
 */

#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <vector>
#include <cstring>
#include <cstdint>

#include "../core/Commons.h"
#include "../core/Console.h"
#include "../core/FileReader.h"
#include "../core/Tokenizer.h"

struct Index
{
	std::vector<int> indices;
	std::vector<int> texCoordIndex;
	std::vector<int> normalIndex;
};

struct Mtl
{
	std::string name, map_Kd_path;
	vec3 ka, kd, ks;

	Mtl(std::string _name = std::string())
	{
		kd = vec3(1.0);
		name = _name;
	}
};

struct Obj
{
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texcoords;
	Mtl mtl;
};

struct FaceGroup
{
	Mtl mtl;
	std::vector<Index> faces;

	FaceGroup(std::vector<Index>& _faces)
	{
		faces = _faces;
	}
};

class ObjLoader
{
public:
	static ObjLoader& getInstance();

	int loadSingleObject(const std::string&, int, Obj&);
	int load(const std::string&, std::vector<Obj>&);
	int loadMtl(const std::string&, std::vector<Mtl>&);
private:
	ObjLoader();
};

#endif
