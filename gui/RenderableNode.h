#ifndef RENDERABLE_NODE_H
#define RENDERABLE_NODE_H

#include "../core/Commons.h"
#include "../scene/SceneNode.h"
#include "../scene/MeshNode.h"

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
