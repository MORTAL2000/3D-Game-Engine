/*
 * Copyright 2015 Alexander Koch
 * File: UserMaterial.h
 * Description: A User defined material (*.mat files).
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

#ifndef USER_MATERIAL_H
#define USER_MATERIAL_H

#include <string>
#include <map>
using std::map;
using std::string;

#include "Material.h"
#include <core/Console.h>
#include <util/GenericType.h>
#include <util/TextureLibrary.h>

typedef map<string, GenericType> AttributeCache;

class UserMaterial : public Material
{
public:
	UserMaterial(const string&);
	~UserMaterial();

	bool contains(const string&);
	GenericType& operator[](const string&);
	void set(const string&, const GenericType&);
	void updateValue(const string&, const GenericType&);
	void setCache(const AttributeCache&);

	string getName();

	virtual void load();
	virtual void update(FilmCamera*, const mat4&);
private:
	AttributeCache m_cache;
	void process(const string&, const GenericType&);
};

#endif
