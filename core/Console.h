/*
 * Copyright 2015 Alexander Koch
 * File: Console.h
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

#ifndef CONSOLE_H
#define CONSOLE_H

#include "Commons.h"

#ifndef __ANDROID_API__
#include <iostream>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <vector>

namespace Console
{
	void setActive(bool);
	void logRaw(const std::string&);
	void log(const char*, ...);
	void logif(bool, const char*, ...);
	void newline();
	void logVector(const std::vector<std::string>&);
	void logVec3(const vec3&);
}

#endif
