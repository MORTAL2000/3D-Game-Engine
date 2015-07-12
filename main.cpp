// Copyright (c) 2014-2015 Alexander Koch

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

int main(int argc, char** argv)
{
    Console::log("///////// Game Engine /////////////////////");
    Console::log("Copyright (c), 2014-2015 Alexander Koch");
    std::vector<std::string> args(argv + 1, argv + argc + !argc);

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
