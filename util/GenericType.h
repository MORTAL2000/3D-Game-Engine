#ifndef GENERIC_TYPE_H
#define GENERIC_TYPE_H

#include <string>
#include <sstream>
#include <vector>

class GenericType
{
public:
	enum Type
	{
		Integer,
		Double,
		Boolean,
		String,
		Vector
	};

	GenericType();
	GenericType(int);
	GenericType(double);
	GenericType(bool);
	GenericType(std::string);
	GenericType(std::vector<double>);

	int getInt() const;
	double getDouble() const;
	bool getBool() const;
	std::string getString() const;
	std::vector<double> getVector() const;

	std::string toString() const;
	Type type() const;
private:
	int m_int;
	double m_double;
	bool m_bool;
	std::string m_string;
	std::vector<double> m_vector;
	Type m_type;
};

#endif
