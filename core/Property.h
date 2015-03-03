 /**
  * @file Property.h
  * @author Alexander Koch
  */

#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include "PropertyParser.h"

class Property
{
public:
	Property(const std::string&);
	~Property();

	operator bool();
	operator int();
	operator float();
	operator std::string();
private:
	std::string m_key;
};

#endif
