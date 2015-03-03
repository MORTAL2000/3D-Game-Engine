#include "Exception.h"

Exception::Exception(const std::string& message) :
	m_message(message)
{}

Exception::~Exception() {}

const std::string Exception::getMessage()
{
	return m_message;
}
