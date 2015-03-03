/**
 * @file Core.h
 * @author Alexander Koch
 */

#ifndef CORE_H
#define CORE_H

#include <vector>
#include <string>

#include "Commons.h"

class Core
{
public:
	Core();

    void setViewport(float, float, float, float);
	void run(const std::vector<std::string>&);
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
	virtual void load(const std::vector<std::string>&) = 0;
};

#endif
