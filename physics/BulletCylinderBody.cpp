/*
 * Copyright 2015 Alexander Koch
 * File: BulletCylinderBody.cpp
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

#include "BulletCylinderBody.h"

BulletCylinderBody::BulletCylinderBody() : m_shape(0), m_half_extent(1.0) {}

void BulletCylinderBody::setHalfExtent(const vec3& extent)
{
    m_half_extent = extent;
}

vec3 BulletCylinderBody::getHalfExtent()
{
    return m_half_extent;
}

void BulletCylinderBody::build(const quat& rotation, const vec3& position)
{
    m_shape = new btCylinderShape(btVector3(m_half_extent.x, m_half_extent.y, m_half_extent.z));
    BulletBody::construct(m_shape, rotation, position);
}
