/*
 * Copyright 2015 Alexander Koch
 * File: Core.h
 * Description: A Core can be run by the context.
 * The core class works as an interface, it has callbacks
 * and methods for processing.
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

#ifndef CORE_H
#define CORE_H

#include <vector>
#include <string>
#include <core/Commons.h>

using std::string;
using std::vector;

class Core {
public:
	Core();

    void setViewport(float, float, float, float);
	void run(const vector<string>&);
	bool isRunning();
    vec4 getViewport();
	void close();
    void setRunning(bool);

	virtual void render() = 0;
	virtual void clear() = 0;

	virtual void onKeyInput(int, int, int, int) {}
    virtual void onCharInput(unsigned int) {}
	virtual void onMouseMovement(double, double) {}
	virtual void onMouseButton(int, int, int) {}
    virtual void onScroll(double, double) {}
    virtual void onResize(int, int) {}
protected:
	vec2 m_dimension;
    vec2 m_offset;
    vec4 m_viewport;
private:
	bool m_running;
	virtual void load(const vector<string>&) = 0;
};

#endif
