/*
 * Copyright 2015 Alexander Koch
 * File: UIFrontend.h
 * Description: Core class for the Frontend / the GUI.
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

#ifndef UI_FRONTEND_H
#define UI_FRONTEND_H

#include <string>
#include <vector>
#include <memory>

#include <core/Commons.h>
#include <core/Core.h>
#include <core/Context.h>
#include <core/FileIO.h>
#include <project/Project.h>
#include <project/SceneManager.h>
#include <rendering/GL.h>
#include <rendering/MeshRenderer.h>
#include <scene/SceneNode.h>
#include <scene/MeshNode.h>
#include <util/CustomCamera.h>
#include <util/MaterialLibrary.h>
#include "RenderableNode.h"

#include <Engine.h>
#include <imgui.h>

// Frontend visual editor
class UIFrontend : public Core
{
public:
    UIFrontend();

    void load(const std::vector<std::string>&);
    void render();
    void clear();

    // callbacks
    void onKeyInput(int, int, int, int);
    void onCharInput(unsigned int);
    void onMouseButton(int, int, int);
    void onMouseMovement(double, double);
    void onScroll(double, double);
    void onResize(int, int);
private:
    mat4 m_mvp;
    vec3 pos;
    float x_rot;
    float y_rot;
    double last_x;
    double last_y;
    int m_button;
    int m_action;

    std::vector<std::shared_ptr<RenderableNode>> m_objects;
    CustomCamera m_camera;
    SceneNode m_scene;

    void resetTransform(const vec3&, const vec3&, const vec3&);
    void updateProperties(SceneNode*);
};

#endif
