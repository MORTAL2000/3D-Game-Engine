#include "Core.h"

Core::Core() : m_dimension(0.0), m_offset(0.0), m_running(false) {}

void Core::setViewport(float x, float y, float width, float height)
{
    m_offset = vec2(x, y);
    m_dimension = vec2(width, height);
    m_viewport = vec4(x, y, width, height);
}

void Core::run(const vector<string>& args)
{
    m_running = true;
    load(args);
}

bool Core::isRunning()
{
    return m_running;
}

vec4 Core::getViewport()
{
    return m_viewport;
}

void Core::close()
{
    m_running = false;
}

void Core::setRunning(bool state)
{
    m_running = state;
}
