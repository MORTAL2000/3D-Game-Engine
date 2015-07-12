#include "Engine.h"
#include <lua/bindings/LuaMath.h>
#include <project/SceneManager.h>
#include <network/Server.h>

Engine::Engine() :
    wireframe(false), postProcess(false), lockCursor(false),
    hasUpdateFunc(false), hasRenderFunc(false), fps(0.0),
    frame(0.0), renderTime(0.0), processingTime(0.0)
{}

void Engine::load(const vector<string>& args)
{
	LuaAPI::getInstance().initialize();

	CFontManager::initialize();
	CFontManager::loadFont("default", "resources/fonts/RobotoCondensed-Bold.ttf", 11);
    CFontManager::loadFont("header", "resources/fonts/SourceSansPro-Regular.otf", 50);

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
    // AudioEngine::getInstance().initialize();
	Console::log("Main: Initialized graphics, materials");

	camera.setPosition(vec3(0, 0, 10));
	camera.setTarget(vec3(0, 0, 0));
	camera.setViewport(m_dimension);
    m_physics.initialize();

	postProcess = Property("postProcessing");
	wireframe = Property("wireframe");
	lockCursor = Property("lockCursor");

    if(lockCursor)
    {
        Context::getInstance().setCursorVisibility(false);
    }

    if(postProcess)
    {
        composer.load(camera);
    }

    // Project initialization
	Console::log("Main: Loading script and project files...");
	string project_file;
    Project project;

    // load project from argument 1
	if(args.size() == 1)
	{
        project.load(args[0]);
        FileIO::setCurrentDirectory(Tokenizer::getDirectory(args[0]));
	}
    else if(args.size() == 2)
    {
        // export current project as package file
        if(args[0] == "-e")
        {
            Console::log("Main: Exporting as package...");
            string temp = Property("project");
            project_file = temp;
            project.load(project_file);
            FileIO::setCurrentDirectory(Tokenizer::getDirectory(project_file));
            project.exportAsRuntime();
            Console::log("Done.");
        }
        // load a package from argument 1
        else if(args[0] == "-p")
        {
            FileReader::setPackage(args[1]);
            Console::log("Main: Loading package: %s", args[1].c_str());

            vector<string> contents;
            auto error = Package::get_files(args[1], contents);
            if(error)
            {
                Console::log("Main: Package: %s", Package::getErrorString(error).c_str());
            }

            for(auto c : contents)
            {
                if(Tokenizer::getFileExtension(c) == "vproj")
                {
                    project_file = c;
                }
            }

            project.load(project_file);
        }
    }
    // load project from properties file
	else
	{
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

void Engine::update()
{
	m_physics.step();
	if(hasUpdateFunc) script.runFunction("onUpdate");

	if(camera.getFlightMode())
	{
		if(actionListener['W']) camera.move(FilmCamera::FORWARD);
		if(actionListener['S']) camera.move(FilmCamera::BACKWARD);
		if(actionListener['A']) camera.move(FilmCamera::STRAFE_LEFT);
		if(actionListener['D']) camera.move(FilmCamera::STRAFE_RIGHT);

		if(actionListener[' ']) camera.move(FilmCamera::UP);
		if(actionListener[340]) camera.move(FilmCamera::DOWN);
	}

	if(actionListener[256])
	{
		close();
	}

    camera.update();
    gl::update(camera.getProjectionMatrix() * camera.getViewMatrix());
}

void Engine::render()
{
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

void Engine::clear()
{
    m_physics.finalize();
    // AudioEngine::getInstance().free();
    MaterialLibrary::getInstance().free();
    LuaAPI::getInstance().finalize();
}

void Engine::renderGeometry()
{
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

void Engine::renderTexture()
{
	if(wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	composer.render();
}

void Engine::renderDebug()
{
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

void Engine::onKeyInput(int key, int scancode, int action, int mods)
{
	actionListener[key] = action;
}

void Engine::onCharInput(unsigned int character)
{

}

void Engine::onMouseMovement(double x, double y)
{
	m_mouse.position = vec2(x,y);

	if(camera.getFlightMode())
	{
		camera.rotate(x, y);
        Context::getInstance().centerCursor();
	}

    m_mouse.last_position = m_mouse.position;
}

void Engine::onMouseButton(int button, int action, int mods)
{
	m_mouse.action = action;
	m_mouse.button = button;
}

void Engine::onResize(int width, int height)
{
	m_dimension = vec2(width, height);
}

FilmCamera* Engine::getDefaultCamera()
{
	return &camera;
}

SceneNode* Engine::getScene()
{
	return &scene;
}

PostComposer* Engine::getComposer()
{
	return &composer;
}

ActionListener* Engine::getActionListener()
{
	return &actionListener;
}

BulletPhysicsEngine* Engine::getPhysicsEngine()
{
	return &m_physics;
}

Mouse* Engine::getMouse()
{
	return &m_mouse;
}
