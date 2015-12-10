/*
 * Copyright 2015 Alexander Koch
 * File: Quad.h
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

#ifndef QUAD_H
#define QUAD_H

#include <vector>

#include <core/Commons.h>
#include <util/Shader.h>
#include "Mesh.h"

struct Quad
{
    Mesh mesh;

    void load()
    {
        std::vector<vec3> vertices;
        vec3 v0 = vec3(0,0,0);
        vec3 v1 = vec3(1,0,0);
        vec3 v2 = vec3(1,1,0);
        vec3 v3 = vec3(0,1,0);
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v2);
        vertices.push_back(v3);
        vertices.push_back(v0);
        mesh.load(vertices);
    }

    void render(Shader* shader) {
        mesh.render(shader);
    }
};

#endif
