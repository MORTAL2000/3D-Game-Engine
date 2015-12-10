/*
 * Copyright 2015 Alexander Koch
 * File: Profiler.h
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

#ifndef PROFILER_H
#define PROFILER_H

#include <cassert>

#include <core/Commons.h>
#include <core/Console.h>

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
