#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <cstdarg>
#include <algorithm>
#include <cstdio>
#include <string>

namespace StringUtils
{
	/**
	 *	Format a string (vaargs to string)
	 * 	@param format Format to use
	 *	@return formatted string
	 */
	std::string format_str(const char* format, ...);

	/**
	 *	Simplify:
	 *	replaces spaces with underscores and
	 * 	converts every character to lowercase
	 *	@param source Source input
	 *	@return simplified string
	 */
	std::string simplify(const std::string& source);

	/**
	 *	Converts a whole string to lowercase
	 *  @param str Source input
	 *	@return lowercase string
	 */
	std::string toLower(const std::string& str);
}

#endif
