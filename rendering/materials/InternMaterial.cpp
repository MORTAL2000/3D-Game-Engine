/*
 * Copyright 2015 Alexander Koch
 * File: InternMaterial.cpp
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

#include "InternMaterial.h"

InternMaterial::InternMaterial() {}

InternMaterial::~InternMaterial() {}

void InternMaterial::load(const std::string& vsh, const std::string& fsh)
{
	m_shader->load(vsh, fsh);
}

void InternMaterial::load() {}

void InternMaterial::update(FilmCamera* camera, const mat4& modelMatrix)
{
	Material::update(camera, modelMatrix);
}
