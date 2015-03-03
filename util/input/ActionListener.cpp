#include "ActionListener.h"

ActionListener::ActionListener()
{}

int& ActionListener::operator[](char code)
{
	return m_map[code];
}

int& ActionListener::operator[](int code)
{
	return m_map[static_cast<char>(code)];
}
