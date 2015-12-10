/*
 * Copyright 2015 Alexander Koch
 * File: Frustum.h
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

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <core/Commons.h>
#include <rendering/BoundingBox.h>

class Frustum
{
public:
	Frustum();
	Frustum(const mat4& viewMatrix);

	void calculate(const mat4& viewMatrix);
	bool boxInFrustum(const BoundingBox& bbox);

private:
	vec4 m_planes[6];
};

#endif
