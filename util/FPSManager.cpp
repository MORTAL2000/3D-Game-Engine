/*
 * Copyright 2015 Alexander Koch
 * File: FPSManager.cpp
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

#include "FPSManager.h"

FPSManager::FPSManager() : m_lastTime(0), m_currentTime(0) {}

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
