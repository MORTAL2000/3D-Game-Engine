/**
 * @file Context.h
 * @author Alexander Koch
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <memory>
#include <vector>
using std::string;
using std::vector;

#include <core/Commons.h>
#include <core/Core.h>
#include <core/Exception.h>
#include <core/Console.h>

#include <core/PropertyParser.h>
#include <core/Property.h>

/**
 *	Context class handles OpenGL context
 *  and window creation. Also loads a properties file.
 */
class Context
{
public:
	static Context& getInstance();

	bool load(const string&, const string&, bool);
	void run(Core*, const vector<string>&);
	void changeCore(Core*, const vector<string>&);

	float getWidth();
	float getHeight();
	vec2 getDimension();
	vec2 getWindowSize();
	vec2 getFramebufferSize();

	void setCursorPosition(const vec2&);
	vec2 getCursorPosition();
	void setCursorVisibility(bool);
	void centerCursor();
	void updateTitle(const string&);
	void setClipboardContent(const string&);
	string getClipboardContent();

	void swapBuffers();
	void takeScreenshot(const string&);

	Core* getCore();
	GLFWwindow* getWindow();
private:
	int m_width;
	int m_height;
	GLFWwindow* m_window;
	Core* m_core;

	Context();
	void initialize(const string&, const string&);

	static void onKeyInput(GLFWwindow*, int, int, int, int);
	static void onCharInput(GLFWwindow*, unsigned int);
	static void onMouseMovement(GLFWwindow*, double, double);
	static void onMouseButton(GLFWwindow*, int, int, int);
	static void onScroll(GLFWwindow*, double, double);
	static void onResize(GLFWwindow*, int, int);
	static void getErrorGLFW(int, const char*);
	void testOpenGL();
	bool checkGL();
};

#endif
