#ifndef PROFILER_H
#define PROFILER_H

#include <cassert>

#include "../core/Commons.h"
#include "../core/Console.h"

class Profiler
{
public:
	Profiler();

	void startInvocation();
	void stopInvocation();

	void displayResult();
	double getResult();
private:
	int invocations;
	double totalTime;
	double startTime;
};

#endif
