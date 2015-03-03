#include "Profiler.h"

Profiler::Profiler()
	: invocations(0), totalTime(0.0), startTime(0.0)
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
