/*
 * Copyright 2015 Alexander Koch
 * File: main.cpp
 * Description: Program entry.
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

#define ENGINE_TITLE "Game Engine v5.6"
#define EDITOR_TITLE "Frontend Editor v5.6"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <core/Core.h>
#include <core/Context.h>
#include <core/Exception.h>
#include <core/Console.h>

#include "Engine.h"
#include <gui/UIFrontend.h>

using std::string;
using std::vector;

int main(int argc, char** argv)
{
    Console::log("///////// Game Engine /////////////////////");
    Console::log("Copyright (c), 2014-2015 Alexander Koch");
    vector<string> args(argv + 1, argv + argc + !argc);

#ifdef BUILD_ENGINE
    Engine engine;
    if(!Context::getInstance().load(ENGINE_TITLE, "engine.properties", false)) exit(1);
    Context::getInstance().run(&engine, args);
#endif

#ifdef BUILD_EDITOR
    UIFrontend editor;
    if(!Context::getInstance().load(EDITOR_TITLE, "editor.properties", false)) exit(1);
    Context::getInstance().run(&editor, args);
#endif

	return 0;
}
