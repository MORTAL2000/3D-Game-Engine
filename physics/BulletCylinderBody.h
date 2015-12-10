/*
 * Copyright 2015 Alexander Koch
 * File: BulletCylinderBody.h
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

#ifndef BULLET_CYLINDER_BODY_H
#define BULLET_CYLINDER_BODY_H

#include "BulletBody.h"

class BulletCylinderBody : public BulletBody
{
public:
    BulletCylinderBody();

    // half extent (x radius, y height, z radius)
    void setHalfExtent(const vec3&);
    vec3 getHalfExtent();

    void build(const quat&, const vec3&);
private:
    btCylinderShape* m_shape;
    vec3 m_half_extent;
};


#endif
