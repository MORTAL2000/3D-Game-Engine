#include "Property.h"

Property::Property(const std::string& key) : m_key(key)
{}

Property::operator bool()
{
	bool value = false;
	PropertyParser::getInstance().getProperty(m_key, value);
	return value;
}

Property::operator int()
{
	int value = 0;
	PropertyParser::getInstance().getProperty(m_key, value);
	return value;
}

Property::operator float()
{
	float value = 0;
	PropertyParser::getInstance().getProperty(m_key, value);
	return value;
}

Property::operator std::string()
{
	std::string value = "";
	PropertyParser::getInstance().getProperty(m_key, value);
	return value;
}
