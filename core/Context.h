/*
 * Copyright 2015 Alexander Koch
 * File: Context.h
 * Description: GLFW OpenGL context loader.
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

// Context class handles OpenGL context
// and window creation. Also loads a properties file.
class Context {
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

	void addDrawCall();
	size_t getDrawCalls();

	Core* getCore();
	GLFWwindow* getWindow();
private:
	int m_width;
	int m_height;
	GLFWwindow* m_window;
	Core* m_core;
 	size_t m_drawCalls;

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
