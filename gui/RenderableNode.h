/*
 * Copyright 2015 Alexander Koch
 * File: RenderableNode.h
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

#ifndef RENDERABLE_NODE_H
#define RENDERABLE_NODE_H

#include <core/Commons.h>
#include <scene/SceneNode.h>
#include <scene/MeshNode.h>

namespace RenderTypes
{
	static std::string Typenames[3] = {"Cube", "Plane", "Mesh"};
}

namespace ColliderTypes
{
	static std::string ColliderNames[3] = {"Box", "Sphere", "Mesh"};
}

struct PhysicsInfo
{
	double mass;
	double friction;
	double restitution;
	vec3 scale;
};

class RenderableNode : public SceneNode
{
public:
	enum Type { CUBE, PLANE, MESH };
	enum Collision { NONE, STATIC, DYNAMIC };
	enum Collider { BOX, SPHERE, TRIANGLE_MESH };

	RenderableNode();

	MeshRenderer* getRenderer();
	void setSelected(bool);
	bool getIsSelected();

	void setType(const Type&);
	Type getType();

	void setCollisionType(const Collision&);
	Collision getCollisionType();

	void setColliderType(const Collider&);
	Collider getColliderType();

	void setMeshPath(const std::string&);
	std::string getMeshPath();

	void setMass(double);
	void setFriction(double);
	void setRestitution(double);
	void setColliderScale(const vec3&);
	PhysicsInfo getPhysicsInfo();

	virtual void draw(const mat4& transform, FilmCamera* camera);
	virtual void draw(const mat4& transform, Shader* shader);
private:
	MeshRenderer m_renderer;
	bool m_selected;
	Type m_type;
	Collision m_collision;
	Collider m_collider;
	std::string m_path;
	PhysicsInfo m_physics;
};

#endif
