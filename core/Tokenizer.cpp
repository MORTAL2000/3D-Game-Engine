#include "Tokenizer.h"

namespace Tokenizer
{
	std::vector<std::string> tokenize(std::string str, char delimiter)
	{
		std::vector<std::string> values;
		size_t delimiters = 0, j, i = 0;
		for(j = 0; j < str.size(); j++) if(str[j] == delimiter) delimiters++;
		if(delimiters == 0) return std::vector<std::string>();
		for(j = 0; j < delimiters; j++)
		{
			std::string value;
			while(str[i] != delimiter && i < str.size())
			{
				value += str[i];
				i++;
			}
			i++;
			values.push_back(value);
		}

		std::string lastVal;
		for(j = i; j < str.size(); j++) lastVal += str[j];
		values.push_back(lastVal);
		return values;
	}

	/**
	 *	Returns extension without the dot
	 */
	std::string getFileExtension(const std::string& name)
	{
		unsigned found = name.find_last_of(".");
		if(found == (unsigned)-1) return std::string();
		return name.substr(found+1);
	}

	std::string getDirectory(const std::string& path)
	{
		return getDirectory(path, false);
	}

	/**
	 *	Removes last directory from file / the file
	 *  slash -> add '/' to the end
	 */
	std::string getDirectory(const std::string& path, bool slash)
	{
		unsigned found = path.find_last_of("\\/");
		if(found == (unsigned)-1) return std::string();
		return path.substr(0, found + (slash? 1 : 0));
	}

	std::string removePath(const std::string& name)
	{
		unsigned found = name.find_last_of("\\/");
		if(found == (unsigned)-1) return std::string();
		return name.substr(found+1);
	}

	std::string removeWhitespace(const std::string& source)
	{
		std::string out;
		for(auto i = 0; i < source.size(); i++)
		{
			if(!std::isspace(source[i]))
			{
				out += source[i];
			}
		}
		return out;
	}

	std::vector<std::string> parseLines(const std::string& source)
	{
		std::vector<std::string> lines;
		std::string current;
		for(auto i = 0; i < source.size(); i++)
		{
			if(source[i] == '\r') continue;
			if(source[i] == '\n')
			{
				lines.push_back(current);
				current.clear();
			}
			else current += source[i];

			if(i == source.size()-1) lines.push_back(current); //last line without '\n' should be uploaded
		}
		return lines;
	}

	std::string removeCharacter(const std::string& source, char ch)
	{
		std::string result;
		for(auto t = 0; t < source.size(); t++)
		{
			if(source[t] != ch)
			result += source[t];
		}
		return result;
	}

	std::vector<std::size_t> findAll(const std::string& source, const std::string& expression)
	{
		std::vector<std::size_t> positions;
		size_t found = source.find(expression, 0);
		while(found != std::string::npos)
		{
		    positions.push_back(found);
		    found = source.find(expression,found+1);
		}
		return positions;
	}

	std::vector<std::size_t> findAllDigits(const std::string& source)
	{
		std::vector<std::size_t> positions;
		for(auto i = 0; i < source.size(); i++)
		{
			if(isdigit(source[i]))
			{
				positions.push_back(i);
			}
		}
		return positions;
	}
}
