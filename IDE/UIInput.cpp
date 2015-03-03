#include "UIInput.h"

UIInput::UIInput() : m_value("1.000")
{

}

UIInput::~UIInput()
{

}

void UIInput::initialize()
{
	m_image.setColor(vec4(1.0));
	m_image.loadFromFile("resources/gui/input.png");
	m_image.setScale(vec2(80, 20));
}

void UIInput::setPosition(const vec2& position)
{
	m_position = position;
	m_image.setPosition(position);
}

void UIInput::setScale(const vec2& scale)
{
	m_scale = scale;
	m_image.setScale(scale);
}

void UIInput::processKeyInput(int key, int scancode, int action, int mods)
{
	if(key == 259 && action == 1)
	{
		m_value.erase(m_value.end() - 1);
	}
}

void UIInput::processCharInput(unsigned int ch)
{
	m_value += ch;
}

void UIInput::render(const vec2& dimension)
{
	m_image.render(dimension);
}

std::string UIInput::getValue()
{
	return m_value;
}
