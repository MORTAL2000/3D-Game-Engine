/*
 * Copyright 2015 Alexander Koch
 * File: MeshBuilder.h
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

#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include <core/Commons.h>
#include <tinyobjloader/tiny_obj_loader.h>
#include <scene/SceneNode.h>

class MeshBuilder
{
public:
	static MeshBuilder& getInstance();
	void load(SceneNode* scene, const char* path);

private:
	struct Material
	{
		vec3 color;
		vec3 ambient;
		vec3 emission;

		Material(const vec3& c, const vec3& a, const vec3& e)
		{
			color = c;
			ambient = a;
			emission = e;
		}

		Material() {}
	};

	struct Triangle
	{
		vec3 v0;
		vec3 v1;
		vec3 v2;
		Material mat;

		Triangle(const vec3& v0, const vec3& v1, const vec3& v2)
		: v0(v0), v1(v1), v2(v2) {}
	};

	MeshBuilder() {}
};

#endif
