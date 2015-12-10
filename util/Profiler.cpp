/*
 * Copyright 2015 Alexander Koch
 * File: Profiler.cpp
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

#include "Profiler.h"

Profiler::Profiler() :
	invocations(0), totalTime(0.0), startTime(0.0)
{}

void Profiler::startInvocation() {
	startTime = glfwGetTime();
}

void Profiler::stopInvocation() {
	if(startTime == 0.0) {
		Console::log("Stop invocation called without starting and invocation");
		assert(startTime != 0.0);
	}
	invocations++;
	totalTime += glfwGetTime() - startTime;
	startTime = 0.0;
}

void Profiler::displayResult() {
	Console::log("Total time for process : %f ms", getResult());
}

double Profiler::getResult() {
	if(totalTime == 0.0) {
		Console::log("Start an invocation and stop after to get a result");
		assert(totalTime != 0);
	}
	float value = (1000.0 * totalTime)/invocations;
	totalTime = 0.0;
	invocations = 0;
	return value;
}
