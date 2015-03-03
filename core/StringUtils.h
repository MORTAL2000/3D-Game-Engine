#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <cstdarg>
#include <algorithm>
#include <cstdio>
#include <string>

namespace StringUtils
{

	std::string format_str(const char*, ...);
	std::string simplify(const std::string&);
	std::string toLower(const std::string&);
}

#endif
