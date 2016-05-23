/*
 * Copyright 2015 Alexander Koch
 * File: Engine.h
 * Description: Engine class definition.
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

#ifndef ENGINE_H
#define ENGINE_H

#include <algorithm>

#include <core/Commons.h>
#include <core/Core.h>
#include <core/FileIO.h>
#include <core/Context.h>
#include <core/Property.h>

#include <rendering/PostComposer.h>
#include <rendering/GL.h>

#include <util/FPSManager.h>
#include <util/Profiler.h>
#include <util/Framebuffer.h>
#include <util/MaterialLibrary.h>
#include <util/input/ActionListener.h>
#include <lua/LuaAPI.h>
#include <lua/LuaScript.h>
#include <lua/bindings/LuaMesh.h>
#include <lua/bindings/LuaMatrix.h>

#include <physics/BulletPhysicsEngine.h>

#include <project/Project.h>
#include <scene/SceneNode.h>

#include <gui/UIFrontend.h>
#include <gui/CFontManager.h>
#include <audio/AudioEngine.h>

using std::string;
using std::vector;

class Engine : public Core {
public:
	Engine();

	void load(const vector<string>&);
	void update();
	void render();
	void clear();

	void onKeyInput(int, int, int, int);
	void onCharInput(unsigned int);
	void onMouseMovement(double, double);
	void onMouseButton(int, int, int);
	void onResize(int, int);

	FilmCamera* getDefaultCamera();
 	SceneNode* getScene();

	PostComposer* getComposer();
	ActionListener* getActionListener();
	BulletPhysicsEngine* getPhysicsEngine();
	Mouse* getMouse();
private:
	bool wireframe;
	bool postProcess;
	bool lockCursor;
	bool hasUpdateFunc;
	bool hasRenderFunc;

	FilmCamera camera;
	SceneNode scene;
	PostComposer composer;

	double fps;
	int frame;
	double renderTime;
	double processingTime;
	Profiler profiler;

	LuaScript script;
	ActionListener actionListener;
	Mouse m_mouse;

	BulletPhysicsEngine m_physics;

	void renderGeometry();
	void renderTexture();
	void renderDebug();
};

#endif
