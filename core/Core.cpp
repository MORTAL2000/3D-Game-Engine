/*
 * Copyright 2015 Alexander Koch
 * File: Core.cpp
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

#include "Core.h"

Core::Core() : m_dimension(0.0), m_offset(0.0), m_running(false) {}

void Core::setViewport(float x, float y, float width, float height) {
    m_offset = vec2(x, y);
    m_dimension = vec2(width, height);
    m_viewport = vec4(x, y, width, height);
}

void Core::run(const vector<string>& args) {
    m_running = true;
    load(args);
}

bool Core::isRunning() {
    return m_running;
}

vec4 Core::getViewport() {
    return m_viewport;
}

void Core::close() {
    m_running = false;
}

void Core::setRunning(bool state) {
    m_running = state;
}
