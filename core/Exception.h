#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception
{
public:
	Exception(const std::string&);
	~Exception();

	const std::string getMessage();
private:
	std::string m_message;
};

#endif
