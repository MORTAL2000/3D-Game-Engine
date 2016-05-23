/*
 * Copyright 2015 Alexander Koch
 * File: Engine.cpp
 * Description: Engine class implementation.
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

#include "Engine.h"
#include <lua/bindings/LuaMath.h>
#include <project/SceneManager.h>
#include <network/Server.h>

Engine::Engine() : wireframe(false), postProcess(false), lockCursor(false),
    hasUpdateFunc(false), hasRenderFunc(false), fps(0.0),
    frame(0.0), renderTime(0.0), processingTime(0.0)
{}

void Engine::load(const vector<string>& args) {
	LuaAPI::getInstance().initialize();

    string font1 = Property("font1");
    string font2 = Property("font2");

	CFontManager::initialize();
    CFontManager::loadFont("default", font1, 11);
    CFontManager::loadFont("header", font2, 50);

    AudioEngine::getInstance().initialize();

	// Graphics initialization
    gl::init();
    gl::viewport(m_viewport);
    gl::clearColor(vec4(0.05, 0.05, 0.05, 1.0));
    gl::clear();

    m_mouse.position = Context::getInstance().getCursorPosition();

    // Display loading message
    glDisable(GL_CULL_FACE);
	CFont* font = CFontManager::getFont("header");
    string cnt = "Game Engine v5.6\nLoading assets ...";
    int fw = font->getStringWidth(cnt);
    int fh = font->getStringHeight(cnt);
    int xw = fw / 4;
    int yw = fh / 2;

	font->renderf(vec2(m_dimension.x / 2 - xw, m_dimension.y / 2 - yw), m_dimension, cnt.c_str());
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    Context::getInstance().swapBuffers();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_FLAT);
	glEnable(GL_FRAMEBUFFER_SRGB);
	MaterialLibrary::getInstance().initialize();
	Console::log("Main: Initialized graphics, materials");

	camera.setPosition(vec3(0, 0, 10));
	camera.setTarget(vec3(0, 0, 0));
	camera.setViewport(m_dimension);
    m_physics.initialize();

	postProcess = Property("postprocessing");
	wireframe = Property("wireframe");
	lockCursor = Property("lockCursor");

    if(lockCursor) {
        Context::getInstance().setCursorVisibility(false);
    }

    if(postProcess) {
        composer.load(camera);
    }

    // Project initialization
	Console::log("Main: Loading script and project files...");
	string project_file;
    Project project;

    // load project from argument 1
	if(args.size() == 1) {
        project.load(args[0]);
        FileIO::setCurrentDirectory(Tokenizer::getDirectory(args[0]));
	}
	else {
		string temp = Property("project");
		project_file = temp;
        project.load(project_file);
        FileIO::setCurrentDirectory(Tokenizer::getDirectory(project_file));
	}

    // load main script
	LuaAPI::getInstance().load(project.getScript(), script);
	Context::getInstance().updateTitle(project.getTitle());
	hasUpdateFunc = script.hasFunction("onUpdate");
    hasRenderFunc = script.hasFunction("onDraw");

	Console::log("Main: Loaded project, running lua init");
	if(script.hasFunction("onInit"))
	{
		script.runFunction("onInit");
	}
	Console::log("Main: Finished, creating context...");
}

void Engine::update() {
	m_physics.step();
	if(hasUpdateFunc) script.runFunction("onUpdate");

	if(camera.getFlightMode()) {
		if(actionListener['W']) camera.move(FilmCamera::FORWARD);
		if(actionListener['S']) camera.move(FilmCamera::BACKWARD);
		if(actionListener['A']) camera.move(FilmCamera::STRAFE_LEFT);
		if(actionListener['D']) camera.move(FilmCamera::STRAFE_RIGHT);

        // Space and shift
		if(actionListener[' ']) camera.move(FilmCamera::UP);
		if(actionListener[340]) camera.move(FilmCamera::DOWN);
	}

    // Escape
	if(actionListener[256]) {
		close();
	}

    camera.update();
    gl::update(camera.getProjectionMatrix() * camera.getViewMatrix());
}

void Engine::render() {
	update();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	profiler.startInvocation();
	renderGeometry();
	profiler.stopInvocation();
	renderTime = profiler.getResult();

	profiler.startInvocation();
	renderTexture();
	profiler.stopInvocation();
	processingTime = profiler.getResult();

	renderDebug();
	fps = FPSManager::getInstance().tick();
}

void Engine::clear() {
    AudioEngine::getInstance().free();
    m_physics.finalize();
    MaterialLibrary::getInstance().free();
    LuaAPI::getInstance().finalize();
}

void Engine::renderGeometry() {
	composer.bind();
	if(wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	gl::clear();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    gl::viewport(m_viewport);

	scene.draw(&camera);
    if(hasRenderFunc) script.runFunction("onDraw");

	composer.unbind();
}

void Engine::renderTexture() {
	if(wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	composer.render();
}

void Engine::renderDebug() {
    size_t dc = Context::getInstance().getDrawCalls();

    glDisable(GL_CULL_FACE);
	string text = "FPS : %.3f  Frame : %i  Position : %.3f %.3f %.3f\nRenderTime : %.3f ms  ProcessingTime : %.3f ms\nDraw Calls: %d\nEngine demo @Alexander Koch 2015";
	vec3 pos = camera.getPosition();
	CFont* font = CFontManager::getFont("default");
	font->renderf(vec2(10, 20), m_dimension, text.c_str(), fps, frame, pos.x, pos.y, pos.z, renderTime, processingTime, dc);
	frame++;
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

void Engine::onKeyInput(int key, int scancode, int action, int mods) {
	actionListener[key] = action;
}

void Engine::onCharInput(unsigned int character) {
    // TODO: Some gui code here for in-game GUIs
}

void Engine::onMouseMovement(double x, double y) {
	m_mouse.position = vec2(x,y);

	if(camera.getFlightMode()) {
		camera.rotate(x, y);
        Context::getInstance().centerCursor();
	}

    m_mouse.last_position = m_mouse.position;
}

void Engine::onMouseButton(int button, int action, int mods) {
	m_mouse.action = action;
	m_mouse.button = button;
}

void Engine::onResize(int width, int height) {
	m_dimension = vec2(width, height);
}

FilmCamera* Engine::getDefaultCamera() {
	return &camera;
}

SceneNode* Engine::getScene() {
	return &scene;
}

PostComposer* Engine::getComposer() {
	return &composer;
}

ActionListener* Engine::getActionListener() {
	return &actionListener;
}

BulletPhysicsEngine* Engine::getPhysicsEngine() {
	return &m_physics;
}

Mouse* Engine::getMouse() {
	return &m_mouse;
}
