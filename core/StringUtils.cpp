#include "StringUtils.h"

namespace StringUtils
{

	std::string format_str(const char* format, ...)
	{
		char buffer[256];
		va_list args;
		va_start(args, format);
		vsprintf(buffer, format, args);
		va_end(args);
		return std::string(buffer);
	}

	// lower case + replace whitespace by underscore
	std::string simplify(const std::string& source)
	{
		std::string temp = source;
		std::replace_if(temp.begin(), temp.end(), isspace, '_');
		return toLower(temp);
	}

	std::string toLower(const std::string& str)
	{
		std::string data = str;
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}

}
