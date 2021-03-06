/*
 * Copyright 2015 Alexander Koch
 * File: SceneNode.h
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

#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <core/Commons.h>
#include <core/Console.h>
#include <util/FilmCamera.h>
#include <util/Shader.h>

#include <vector>
#include <string>
using std::vector;
using std::string;

class SceneNode
{
public:
	typedef vector<SceneNode*> NodePointerList;
	enum Type { EMPTY, MESH };

	SceneNode();
	virtual ~SceneNode() {}

	virtual void draw(const mat4& transform, FilmCamera* camera);
	virtual void draw(const mat4& transform, Shader* shader);
	void draw(FilmCamera* camera);

	void setId(long);
	long getId();

	void setName(const string&);
	string getName();

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
	string m_name;
	NodePointerList m_children;
	Type m_type;
};

#endif
