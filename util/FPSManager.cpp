#include "FPSManager.h"

FPSManager::FPSManager() :
	m_lastTime(0), m_currentTime(0)
{}

/* Public methods */
FPSManager& FPSManager::getInstance()
{
	static FPSManager instance;
	return instance;
}

void FPSManager::init()
{
	m_lastTime = glfwGetTime();
}

double FPSManager::getDeltaTime()
{
	m_currentTime = glfwGetTime();
	double delta = m_currentTime - m_lastTime;
	m_lastTime = m_currentTime;
	return delta;
}

double FPSManager::tick()
{
	static double timeValue = glfwGetTime(), fps = 0;
	static int frameCount = 0;
	double current = glfwGetTime(), timeInterval = 1;

	if((current - timeValue) > timeInterval) {
		fps = double(frameCount/(current-timeValue));
		frameCount = 0;
		timeValue = glfwGetTime();
	}
	else frameCount++;

	return fps;
}
