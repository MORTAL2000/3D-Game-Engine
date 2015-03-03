#include "GenericType.h"

GenericType::GenericType()
{}

GenericType::GenericType(int value) :
	m_int(value), m_type(GenericType::Type::Integer)
{}

GenericType::GenericType(double value) :
	m_double(value), m_type(GenericType::Type::Double)
{}

GenericType::GenericType(bool value) :
	m_bool(value), m_type(GenericType::Type::Boolean)
{}

GenericType::GenericType(std::string value) :
	m_string(value), m_type(GenericType::Type::String)
{}

GenericType::GenericType(std::vector<double> vector) :
	m_vector(vector), m_type(GenericType::Type::Vector)
{}

int GenericType::getInt() const
{
	return m_int;
}

double GenericType::getDouble() const
{
	return m_double;
}

bool GenericType::getBool() const
{
	return m_bool;
}

std::string GenericType::getString() const
{
	return m_string;
}

std::vector<double> GenericType::getVector() const
{
	return m_vector;
}

std::string GenericType::toString() const
{
	switch(type())
	{
		case GenericType::Type::String:
		{
			return m_string;
		}
		case GenericType::Type::Integer:
		{
			std::ostringstream sstream;
			sstream << m_int;
			return sstream.str();
		}
		case GenericType::Type::Double:
		{
			std::ostringstream sstream;
			sstream << m_double;
			return sstream.str();
		}
		case GenericType::Type::Boolean:
		{
			return m_bool ? "true" : "false";
		}
		case GenericType::Type::Vector:
		{
			std::ostringstream sstream;
			sstream << "[" << m_vector[0];
			for(size_t i = 1; i < m_vector.size(); i++)
			{
				sstream << " " << m_vector[i];
			}
			sstream << "]";
			return sstream.str();
		}
		default:
		{
			return "";
		}
	}
}

GenericType::Type GenericType::type() const
{
	return m_type;
}
