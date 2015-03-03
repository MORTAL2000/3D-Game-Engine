#include "PropertyParser.h"

PropertyParser::PropertyParser()
{
	m_cache["width"] = "1280";
	m_cache["height"] = "720";
	m_cache["hideCursor"] = "1";
}

PropertyParser& PropertyParser::getInstance()
{
	static PropertyParser instance;
	return instance;
}

bool PropertyParser::load(const std::string& filename)
{
	std::vector<std::string> lines;
	if(!FileReader::readLines(filename, &lines))
	{
		Console::log("Error reading file %s", filename.c_str());
		return false;
	}

	for(size_t i = 0; i < lines.size(); i++)
	{
		size_t comment = lines[i].find('#');
		if(comment != std::string::npos) lines[i].erase(comment);
		if(lines[i].size() == 0) continue;

		lines[i] = Tokenizer::removeWhitespace(lines[i]);
		std::vector<std::string> values = Tokenizer::tokenize(lines[i], '=');
		m_cache[values[0]] = values[1];
	}

	return true;
}

/* access methods */
void PropertyParser::getProperty(const std::string& key, bool& value)
{
	int v;
	sscanf(m_cache[key].c_str(), "%i", &v);
	value = (v > 0);
}

void PropertyParser::getProperty(const std::string& key, float& value)
{
	sscanf(m_cache[key].c_str(), "%f", &value);
}

void PropertyParser::getProperty(const std::string& key, int& value)
{
	sscanf(m_cache[key].c_str(), "%i", &value);
}

void PropertyParser::getProperty(const std::string& key, std::string& value)
{
	value = m_cache[key];
}

void PropertyParser::printCache()
{
	Console::log("------ Properties ------");
	for(PropertiesCache::iterator it = m_cache.begin(); it != m_cache.end(); it++)
	Console::log("%s = %s", it->first.c_str(), it->second.c_str());
	Console::log("------ ---------- ------");
}
