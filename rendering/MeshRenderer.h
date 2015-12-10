/*
 * Copyright 2015 Alexander Koch
 * File: MeshRenderer.h
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

#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include <core/Commons.h>
#include <rendering/Mesh.h>
#include <rendering/materials/Material.h>

class MeshRenderer : public Mesh
{
public:
	MeshRenderer();

	void setMaterial(Material*);
	Material* getMaterial();

	void setPosition(const vec3&);
	void setRotation(const vec3&);
	void setRotation(const quat&);
	void setScale(const vec3&);

	vec3 getPosition() const;
	quat getRotation() const;
	vec3 getScale() const;

	void setCullFace(GLenum);
	void setUseDepthMask(bool);
	GLenum getCullFace();
	bool getDepthMaskUsage();

	mat4 getModelMatrix();

	void update(FilmCamera*);
	void update(const mat4&, FilmCamera*);
	void render(Shader*);
	void render();

	int getTriangleCount();
private:
	Material* m_material;

	vec3 m_position;
	quat m_rotation;
	vec3 m_scale;

	GLenum m_cullFace;
	bool m_useDepthMask;
};

#endif
