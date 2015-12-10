/*
 * Copyright 2015 Alexander Koch
 * File: BoundingBox.cpp
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

#include "BoundingBox.h"

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(const vec3& min, const vec3& max) : m_min(min), m_max(max)
{}

void BoundingBox::set(const vec3& min, const vec3& max)
{
	m_min = min;
	m_max = max;
}

bool BoundingBox::containsPoint(const vec3& point)
{
	return point.x >= m_min.x && point.x <= m_max.x &&
			point.y >= m_min.y && point.y <= m_max.y &&
			point.z >= m_min.z && point.z <= m_max.z;
}

bool BoundingBox::contains(const BoundingBox* bbox)
{
	return containsPoint(bbox->getMin()) && containsPoint(bbox->getMax());
}

void BoundingBox::transform(const mat4& mat)
{
	vec4 min4 = mat * vec4(m_min.x, m_min.y, m_min.z, 1.0f);
	vec4 max4 = mat * vec4(m_max.x, m_max.y, m_max.z, 1.0f);
	m_min = vec3(min4);
	m_max = vec3(max4);
}

vec3 BoundingBox::getMin() const
{
	return m_min;
}

vec3 BoundingBox::getMax() const
{
	return m_max;
}
