#include "Property.h"

Property::Property(const string& key) : m_key(key) {}

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

Property::operator string()
{
	string value = "";
	PropertyParser::getInstance().getProperty(m_key, value);
	return value;
}
