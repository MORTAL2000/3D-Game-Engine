 /**
  * @file Property.h
  * @author Alexander Koch
  */

#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include "PropertyParser.h"
using std::string;

class Property
{
public:
	Property(const string&);

	operator bool();
	operator int();
	operator float();
	operator string();
private:
	std::string m_key;
};

#endif
