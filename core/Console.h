#ifndef CONSOLE_H
#define CONSOLE_H

#include "Commons.h"

#ifndef __ANDROID_API__
#include <iostream>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string>

namespace Console
{
	void setActive(bool);
	void logRaw(const std::string&);
	void log(const char*, ...);
	void logif(bool, const char*, ...);
	void newline();
}

#endif
