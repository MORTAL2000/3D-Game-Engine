/*
 * Copyright 2015 Alexander Koch
 * File: Material.h
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

#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <string>
#include <memory>

#include <core/Commons.h>
#include <util/FilmCamera.h>
#include <util/Shader.h>
#include <util/ITexture.h>

class Material
{
public:
    Material();
    ~Material();

    virtual void load();
    virtual void update(FilmCamera*, const mat4&);

    Shader* getShader();

    void bindTextures();
    void unbindTextures();
protected:
    std::shared_ptr<Shader> m_shader;
    std::map<std::string, ITexture*> m_texture_cache;
};

#endif
