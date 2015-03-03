#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "../core/Commons.h"
#include "../core/Console.h"
#include "../util/FilmCamera.h"
#include "../util/Shader.h"

#include <vector>
#include <string>

class SceneNode
{
public:
	typedef std::vector<SceneNode*> NodePointerList;
	enum Type { EMPTY, MESH };

	SceneNode();
	virtual ~SceneNode() {}

	virtual void draw(const mat4& transform, FilmCamera* camera);
	virtual void draw(const mat4& transform, Shader* shader);
	void draw(FilmCamera* camera);

	void setId(long);
	long getId();

	void setName(const std::string&);
	std::string getName();

	Type getType();

	void addChild(SceneNode* node);
	void removeChild(SceneNode* node);
	NodePointerList getChildren();
	long getChildrenCount();
	void clear();
protected:
	long m_id;
	long m_id_counter;
	mat4 m_transformation;
	std::string m_name;
	NodePointerList m_children;
	Type m_type;
};

#endif
