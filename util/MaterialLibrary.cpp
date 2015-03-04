#include "MaterialLibrary.h"
#include "lua/LuaScript.h"

MaterialLibrary::MaterialLibrary()
{}

MaterialLibrary& MaterialLibrary::getInstance()
{
	static MaterialLibrary instance;
	return instance;
}

void MaterialLibrary::initialize()
{
	m_default.load("shader/diffuse.vsh", "shader/diffuse.fsh");
	Shader* shdr = m_default.getShader();
	shdr->bind();
	shdr->vec3f("direction", vec3(-0.2, 0.8, 0.5));
	shdr->vec4f("color", vec4(1.0));
	shdr->unbind();
}

bool MaterialLibrary::load(const std::string& filename)
{
	LuaScript script(filename);
	auto tex = script.getTableKeys("textures");
	for(auto i = 0; i < tex.size(); i++)
	{
		std::string key = tex[i];
		std::string value = script.get<std::string>("textures."+tex[i]);

		if(!TextureLibrary::getInstance().load(key, value))
		{
			Console::log("WARNING: Could not load %s with value %s", key.c_str(), value.c_str());
		}
	}

	// test cubemaps TODO
	/*auto cubes = script.getTableKeys("cubemaps")
	for(auto i = 0; i < cubes.size(); i++)
	{
		std::string key = cubes[i];
		std::string posx = script.get<std::string>("cubemaps."+cubes[i]+".posX");
	}*/

	auto mat = script.getTableKeys("materials");
	for(auto i = 0; i < mat.size(); i++)
	{
		// for each material
		AttributeCache cache;
		auto sub = script.getTableKeys("materials."+mat[i]);
		for(auto j = 0; j < sub.size(); j++)
		{
			// for each sub entry in material
			std::string key = sub[j];
			auto type = script.getType("materials."+mat[i]+"."+sub[j]);
			switch(type)
			{
				case LUA_TSTRING:
				{
					std::string str = script.get<std::string>("materials."+mat[i]+"."+sub[j]);
					if(key == "shader")
					{
						cache[key] = GenericType(str);
					}
					break;
				}
				case LUA_TNUMBER:
				{
					float value = script.get<float>("materials."+mat[i]+"."+sub[j]);
					cache[key] = GenericType(value);
					break;
				}
				case LUA_TTABLE:
				{
					std::vector<float> table = script.getNumberVector("materials."+mat[i]+"."+sub[j]);
					std::vector<double> vec;
					for(auto f : table)
					{
						vec.push_back((double)f);
					}

					cache[key] = GenericType(vec);
					break;
				}
			}
		}

		cache["name"] = mat[i];
		std::shared_ptr<UserMaterial> material(new UserMaterial(mat[i]));
		material->setCache(cache);
		material->load();
		materials.push_back(material);
	}
	script.close();
	return true;
}

UserMaterial* MaterialLibrary::get(const std::string& material)
{
	for(auto i = 0; i < materials.size(); i++)
	{
		if(materials[i]->getName() == material)
		{
			return materials[i].get();
		}
	}
	return 0;
}

Material* MaterialLibrary::getDefault()
{
	return &m_default;
}

std::vector<std::string> MaterialLibrary::getMaterialNames()
{
	std::vector<std::string> names;
	for(auto material : materials)
	{
		names.push_back(material->getName());
	}
	return names;
}

void MaterialLibrary::free()
{
	TextureLibrary::getInstance().free();
}
