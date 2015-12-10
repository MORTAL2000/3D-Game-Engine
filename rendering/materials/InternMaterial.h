/*
 * Copyright 2015 Alexander Koch
 * File: InternMaterial.h
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

#ifndef INTERN_MATERIAL_H
#define INTERN_MATERIAL_H

#include "Material.h"

class InternMaterial : public Material
{
public:
	InternMaterial();
	~InternMaterial();

	void load(const std::string&, const std::string&);

	virtual void load();
	virtual void update(FilmCamera*, const mat4&);
};

#endif
