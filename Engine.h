/**
 * @file Engine.h
 * @author Alexander Koch
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

/**
 *	Game engine main component
 */
class Engine : public Core
{
public:
	Engine();

	void load(const std::vector<std::string>&);
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
