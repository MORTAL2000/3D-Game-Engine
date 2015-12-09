#include "Context.h"

Context::Context() : m_height(720), m_width(1280), m_window(0), m_core(0) {}

/**
 *  Returns the global Context instance
 */
Context& Context::getInstance()
{
	static Context instance;
	return instance;
}

/**
 * Wrapper for initialization
 *
 * @param title The title for the created window
 * @param properties The file to parse to get creation properties
 * @param silent Disable warning / error and info messages
 */
bool Context::load(const string& title, const string& properties, bool silent)
{
	Console::setActive(!silent);
	try
	{
		initialize(title, properties);
	}
	catch(Exception& e)
	{
		Console::log("A fatal exception was encountered");
		Console::log("%s", e.getMessage().c_str());
		glfwTerminate();
		return false;
	}

	return true;
}

/**
 * Creates the GLFW / OPENGL Context based on platform
 *
 * @param title The title for the created window
 * @param properties The file to parse to get creation properties
 */
void Context::initialize(const string& title, const string& properties)
{
	// load properties file
	if(!PropertyParser::getInstance().load(properties)) throw Exception("Property file not available");

	glfwSetErrorCallback(getErrorGLFW);
	if(!glfwInit()) throw Exception("OpenGL Context (GLFW) initialization failed");

//#define __FORWARD_COMPATIBLE__
#ifdef __FORWARD_COMPATIBLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	if(Property("width")) m_width = Property("width");
	if(Property("height")) m_height = Property("height");
	if(m_width < 1 || m_height < 1) throw Exception("Invalid window size");

	if(Property("fullscreen"))
		m_window = glfwCreateWindow(Property("fullscreenWidth"), Property("fullscreenHeight"), title.c_str(), glfwGetPrimaryMonitor(), 0);
	else
		m_window = glfwCreateWindow(m_width, m_height, title.c_str(), 0, 0);

	if(!m_window) throw Exception("Failed to create an OpenGL window");
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	// set callbacks to the engine
	glfwSetKeyCallback(m_window, onKeyInput);
	glfwSetCharCallback(m_window, onCharInput);
	glfwSetCursorPosCallback(m_window, onMouseMovement);
	glfwSetMouseButtonCallback(m_window, onMouseButton);
	glfwSetScrollCallback(m_window, onScroll);
	glfwSetFramebufferSizeCallback(m_window, onResize);

	// set properties
	if(Property("hideCursor")) setCursorVisibility(false);

#if defined(__WINDOWS_API__) || defined(__LINUX_API__)
	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		glfwDestroyWindow(m_window);
		throw Exception("GLEW initialization failed");
	}

	Console::log("GLEW Version %s", glewGetString(GLEW_VERSION));
#endif

	// print context information
	Console::log("OpenGL Version %s", glGetString(GL_VERSION));
	Console::log("GLSL Version %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	Console::log("Vendor %s", glGetString(GL_VENDOR));
	Console::log("Renderer %s", glGetString(GL_RENDERER));
	testOpenGL();

	// print properties (debug)
	if(Property("print")) PropertyParser::getInstance().printCache();
	glfwGetFramebufferSize(m_window, &m_width, &m_height);
}

/**
 * Runs the specified core
 * @param core The core to run
 */
void Context::run(Core* core, const vector<string>& args)
{
	if(core == 0)
	{
		if(m_window) glfwDestroyWindow(m_window);
		throw Exception("Core is a nullpointer");
	}

	m_core = core;
	m_core->setViewport(0, 0, m_width, m_height);
	m_core->run(args);

	while(m_core->isRunning())
	{
		m_core->render();
		glfwSwapBuffers(m_window);
		glfwPollEvents();

		if(glfwWindowShouldClose(m_window))
		{
			m_core->close();
		}

		m_drawCalls = 0;
	}

	m_core->clear();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

/**
 *	<<Experimental>>
 */
void Context::changeCore(Core* core, const vector<string>& args)
{
	Core* origin = m_core;
	origin->close();

	m_core = core;
	m_core->setViewport(0, 0, m_width, m_height);
	m_core->run(args);
	while(m_core->isRunning())
	{
		m_core->render();
		glfwSwapBuffers(m_window);
		glfwPollEvents();
		if(glfwWindowShouldClose(m_window))
		m_core->close();
	}
	m_core->clear();

	m_core = origin;
	m_core->setRunning(true);
}

float Context::getWidth()
{
	return m_width;
}

float Context::getHeight()
{
	return m_height;
}

vec2 Context::getDimension()
{
	return vec2(m_width, m_height);
}

vec2 Context::getWindowSize()
{
	int w, h;
	glfwGetWindowSize(m_window, &w, &h);
	return vec2(w, h);
}

vec2 Context::getFramebufferSize()
{
	int display_w, display_h;
	glfwGetFramebufferSize(m_window, &display_w, &display_h);
	return vec2(display_w, display_h);
}

void Context::setCursorPosition(const vec2& position)
{
	glfwSetCursorPos(m_window, position.x, position.y);
}

vec2 Context::getCursorPosition()
{
	double mouse_x, mouse_y;
	glfwGetCursorPos(m_window, &mouse_x, &mouse_y);
	return vec2(mouse_x, mouse_y);
}

void Context::setCursorVisibility(bool status)
{
	glfwSetInputMode(m_window, GLFW_CURSOR, status ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Context::centerCursor()
{
	glfwSetCursorPos(m_window, m_width * 0.5, m_height * 0.5);
}

void Context::updateTitle(const string& title)
{
	glfwSetWindowTitle(m_window, title.c_str());
}

void Context::setClipboardContent(const string& content)
{
	glfwSetClipboardString(m_window, content.c_str());
}

string Context::getClipboardContent()
{
	return string(glfwGetClipboardString(m_window));
}

void Context::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

void Context::takeScreenshot(const string& filename)
{
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	int w = getWidth();
	int h = getHeight();

	char* buffer = new char[w*h*3];
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	FILE* fp = fopen(filename.c_str(), "wb");
	if(!fp) return;
	fprintf(fp,"P6\n%d %d\n255\n",w,h);
	for(int y=0;y<h;y++) { /* flip image bottom-to-top on output */
		fwrite(&buffer[3*(h-1-y)*w],1,3*w,fp);
	}
	fclose(fp);
	delete[] buffer;
	Console::log("Saved file as %s", filename.c_str());
}


void Context::addDrawCall()
{
	m_drawCalls++;
}

size_t Context::getDrawCalls()
{
	return m_drawCalls;
}

Core* Context::getCore()
{
	return m_core;
}

GLFWwindow* Context::getWindow()
{
	return m_window;
}

// physical key callback
void Context::onKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	getInstance().m_core->onKeyInput(key, scancode, action, mods);
}

// virtual key callback
void Context::onCharInput(GLFWwindow* window, unsigned int codepoint)
{
	getInstance().m_core->onCharInput(codepoint);
}

// mouse movement callback
void Context::onMouseMovement(GLFWwindow* window, double x, double y)
{
	getInstance().m_core->onMouseMovement(x, y);
}

// mouse button action callback
void Context::onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	getInstance().m_core->onMouseButton(button, action, mods);
}

void Context::onScroll(GLFWwindow* window, double xOffset, double yOffset)
{
	getInstance().m_core->onScroll(xOffset, yOffset);
}

// window resize callback
void Context::onResize(GLFWwindow* window, int width, int height)
{
	getInstance().m_core->onResize(width, height);
}

// glfw error callback
void Context::getErrorGLFW(int error, const char* description)
{
	Console::log("%d: %s", error, description);
}

/**
 * Test the validity of the created OpenGL context
 */
void Context::testOpenGL()
{
	int major, minor, profile;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	Console::log("Loaded OpenGL Version %i.%i", major, minor);
	Console::logif(!checkGL(), "An error occurred querying the OpenGL version");
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	bool condition = (profile == GL_CONTEXT_CORE_PROFILE_BIT);
	Console::logif(condition, "Context is forward compatible/core profile");
	Console::logif(!condition, "Context is not foward compatible");
	Console::logif(!checkGL(), "An error occurred determining the fwcc support");
}

bool Context::checkGL()
{
	return glGetError() == GL_NO_ERROR;
}
