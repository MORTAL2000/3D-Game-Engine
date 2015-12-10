/*
 * Copyright 2015 Alexander Koch
 * File: PostComposer.h
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

#ifndef POST_COMPOSER_H
#define POST_COMPOSER_H

#include <memory>

#include "Quad.h"
#include <core/Property.h>
#include <util/Framebuffer.h>
#include <util/FilmCamera.h>
#include <util/Shader.h>
#include <util/Texture2D.h>

// Special type of Post-Processor
class PostComposer
{
private:
    vec2 dimension;
    bool shaded;
    bool active;

    // Framebuffers
    std::shared_ptr<Framebuffer> mainBuffer;
    std::shared_ptr<Framebuffer> readBuffer;
    std::shared_ptr<Framebuffer> writeBuffer;

    // Shaders
    Shader base;
    Shader dof;
    Shader tonemapper;
    Shader ssao;
    Shader flare, flareCompose;
    Shader bloom;

    // Shaders config
    bool doSSAO;
    bool doDOF;
    bool doLensFlares;
    bool doTonemapping;

    // Dummy texture
    Texture2D dust;

    // Utility
    Quad quad;
    mat4 ortho;
    bool focus;

    // Passes
    void compose();
    void passSSAO();
    void passDoF();
    void passMotionBlur();
    void passTonemap();
    void passLensFlares();
    void passBloom();
    void passFinalRender();

    // Swap the 3 framebuffers
    void swapBuffers();

public:
    PostComposer();
    ~PostComposer();

    void load(FilmCamera&);

    void bind();
    void unbind();

    void setActive(bool);
    bool isActive();

    void render();
    void passCustom();
};

#endif
