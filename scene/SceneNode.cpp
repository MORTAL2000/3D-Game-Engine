/*
 * Copyright 2015 Alexander Koch
 * File: SceneNode.cpp
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

#include "SceneNode.h"

SceneNode::SceneNode() : m_id(-1), m_id_counter(-1), m_transformation(1.0), m_type(EMPTY) {}

void SceneNode::draw(const mat4& parentTransform, FilmCamera* camera)
{
	mat4 childTransform = parentTransform * m_transformation;

	for(NodePointerList::const_iterator ci = m_children.begin(); ci != m_children.end(); ci++)
	{
		(*ci)->draw(childTransform, camera);
	}
}

void SceneNode::draw(const mat4& parentTransform, Shader* shader)
{
	mat4 childTransform = parentTransform * m_transformation;

	for(NodePointerList::const_iterator ci = m_children.begin(); ci != m_children.end(); ci++)
	{
		(*ci)->draw(childTransform, shader);
	}
}

void SceneNode::draw(FilmCamera* camera)
{
	draw(mat4(1.0), camera);
}

void SceneNode::setId(long id)
{
	m_id = id;
}

long SceneNode::getId()
{
	return m_id;
}

void SceneNode::setName(const string& name)
{
	m_name = name;
}

std::string SceneNode::getName()
{
	return m_name;
}

SceneNode::Type SceneNode::getType()
{
	return m_type;
}

void SceneNode::addChild(SceneNode* node)
{
	if(!node)
	{
		Console::log("Tried to add invalid node");
		return;
	}

	node->setId(m_id_counter++);
	m_children.push_back(node);
}

void SceneNode::removeChild(SceneNode* node)
{
	for(auto i = 0; i < m_children.size(); i++)
	{
		if(m_children[i]->getId() == node->getId())
		{
			m_children.erase(m_children.begin() + i);
		}
	}
}

SceneNode::NodePointerList SceneNode::getChildren()
{
	return m_children;
}

long SceneNode::getChildrenCount()
{
	return m_children.size();
}

void SceneNode::clear()
{
	m_children.clear();
}
