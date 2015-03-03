/**
 * @file Context.h
 * @author Alexander Koch
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <memory>
#include <vector>

#include "Commons.h"
#include "Core.h"
#include "Exception.h"
#include "Console.h"

#include "PropertyParser.h"
#include "Property.h"

/**
 *	Context class handles OpenGL context
 *  and window creation. Also loads properties file.
 */
class Context
{
public:
	static Context& getInstance();

	bool load(const std::string&, const std::string&, bool);
	void run(Core*, const std::vector<std::string>&);
	void changeCore(Core*, const std::vector<std::string>&);

	float getWidth();
	float getHeight();
	vec2 getDimension();
	vec2 getWindowSize();
	vec2 getFramebufferSize();

	void setCursorPosition(const vec2&);
	vec2 getCursorPosition();
	void setCursorVisibility(bool);
	void centerCursor();
	void updateTitle(const std::string&);
	void setClipboardContent(const std::string&);
	std::string getClipboardContent();

	void takeScreenshot(const std::string&);

	Core* getCore();
	GLFWwindow* getWindow();
private:
	int m_width;
	int m_height;
	GLFWwindow* m_window;
	Core* m_core;

	Context();
	void initialize(const std::string&, const std::string&);

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
