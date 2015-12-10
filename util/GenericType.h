/*
 * Copyright 2015 Alexander Koch
 * File: GenericType.h
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
