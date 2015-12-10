/*
 * Copyright 2015 Alexander Koch
 * File: BulletSphereBody.cpp
 * Description: Wrapper for the bullet physics engine.
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

#include "BulletSphereBody.h"

BulletSphereBody::BulletSphereBody() : m_shape(0), m_radius(1.0) {}

BulletSphereBody::~BulletSphereBody() {}

void BulletSphereBody::setRadius(double radius)
{
	m_radius = radius;
}

double BulletSphereBody::getRadius()
{
	return m_radius;
}

void BulletSphereBody::build(const quat& rotation, const vec3& position)
{
	m_shape = new btSphereShape(m_radius);
	BulletBody::construct(m_shape, rotation, position);
}
