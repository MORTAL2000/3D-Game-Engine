/*
 * Copyright 2015 Alexander Koch
 * File: MaterialLibrary.cpp
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

#include "MaterialLibrary.h"
#include "lua/LuaScript.h"

MaterialLibrary::MaterialLibrary() {}

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
	if(script.hasVariable("textures"))
	{
		auto tex = script.getTableKeys("textures");
		for(auto i = 0; i < tex.size(); i++)
		{
			std::string key = tex[i];
			std::string value = script.get<std::string>("textures."+tex[i]);

			if(!TextureLibrary::getInstance().load(key, value))
			{
				Console::log("WARNING: Could not load %s with value %s (Texture2D)", key.c_str(), value.c_str());
			}
		}
	}

	// test cubemaps TODO
	if(script.hasVariable("cubemaps"))
	{
		auto cubes = script.getTableKeys("cubemaps");
		for(auto i = 0; i < cubes.size(); i++)
		{
			std::string key = cubes[i];
			std::string posx = script.get<std::string>("cubemaps."+cubes[i]+".posX");
			std::string negx = script.get<std::string>("cubemaps."+cubes[i]+".negX");
			std::string posy = script.get<std::string>("cubemaps."+cubes[i]+".posY");
			std::string negy = script.get<std::string>("cubemaps."+cubes[i]+".negY");
			std::string posz = script.get<std::string>("cubemaps."+cubes[i]+".posZ");
			std::string negz = script.get<std::string>("cubemaps."+cubes[i]+".negZ");

			if(!TextureLibrary::getInstance().load(key, posx, negx, posy, negy, posz, negz))
			{
				Console::log("WARNING: Could not load %s (Cubemap)", key.c_str());
			}
		}
	}

	// there has to be a materials variable
	if(!script.hasVariable("materials")) return false;
	auto mat = script.getTableKeys("materials");
	std::map<std::string, GenericType> assign_textures;

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
					else
					{
						assign_textures[key] = GenericType(str);
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

		// pre-process textures for use
		for(auto tex : assign_textures)
		{
			material->updateValue(tex.first, tex.second);
		}

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
