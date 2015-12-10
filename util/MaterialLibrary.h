/*
 * Copyright 2015 Alexander Koch
 * File: MaterialLibrary.h
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

#ifndef MATERIAL_LIBRARY_H
#define MATERIAL_LIBRARY_H

#include <vector>
#include <string>
#include <memory>

#include <core/Console.h>
#include <rendering/materials/Material.h>
#include <rendering/materials/UserMaterial.h>
#include <rendering/materials/InternMaterial.h>
#include "TextureLibrary.h"

class MaterialLibrary
{
public:
	static MaterialLibrary& getInstance();

	void initialize();
 	bool load(const std::string&);
	UserMaterial* get(const std::string&);
	Material* getDefault();
	std::vector<std::string> getMaterialNames();

	void free();
private:
	MaterialLibrary();

	std::vector<std::shared_ptr<UserMaterial>> materials;
	InternMaterial m_default;
};

#endif
