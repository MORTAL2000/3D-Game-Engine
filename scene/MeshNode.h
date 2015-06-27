#ifndef MESH_NODE_H
#define MESH_NODE_H

#include "SceneNode.h"
#include <rendering/MeshRenderer.h>

class MeshNode : public SceneNode
{
public:
	MeshNode();
	MeshNode(MeshRenderer*);
	~MeshNode() {}

	virtual void draw(const mat4& transform, FilmCamera* camera);
	virtual void draw(const mat4& transform, Shader* shader);
	void setRenderer(MeshRenderer* renderer);
private:
	MeshRenderer* m_renderer;
};

#endif
