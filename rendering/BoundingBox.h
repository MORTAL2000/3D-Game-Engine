/*
 * Copyright 2015 Alexander Koch
 * File: BoundingBox.h
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

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <core/Commons.h>

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(const vec3& min, const vec3& max);

	void set(const vec3& min, const vec3& max);
	bool containsPoint(const vec3& point);
	bool contains(const BoundingBox* bbox);
	void transform(const mat4& mat);

	vec3 getMin() const;
	vec3 getMax() const;

private:
	vec3 m_min;
	vec3 m_max;
};

#endif
