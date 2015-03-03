#ifndef MATERIAL_LIBRARY_H
#define MATERIAL_LIBRARY_H

#include <vector>
#include <string>
#include <memory>

#include "../core/Console.h"
#include "../rendering/materials/Material.h"
#include "../rendering/materials/UserMaterial.h"
#include "../rendering/materials/InternMaterial.h"
#include "TextureLibrary.h"

class MaterialLibrary
{
public:
	static MaterialLibrary& getInstance();

	void initialize();
 	bool load(const std::string&);
	UserMaterial* get(const std::string&);
	Material* getDefault();

	void free();
private:
	MaterialLibrary();

	std::vector<std::shared_ptr<UserMaterial>> materials;
	InternMaterial m_default;
};

#endif
