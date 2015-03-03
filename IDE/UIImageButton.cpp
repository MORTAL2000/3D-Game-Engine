#include "UIImageButton.h"

UIImageButton::UIImageButton()
{

}

UIImageButton::~UIImageButton()
{

}

void UIImageButton::setCallback(Callback callback)
{
	m_callback = callback;
}

void UIImageButton::processMouse(Mouse* mouse)
{
	vec2 position = mouse->position;
	bool condition0 = position.x > m_position.x && position.x < m_position.x + m_scale.x;
	bool condition1 = position.y > m_position.y && position.y < m_position.y + m_scale.y;
	bool over = condition0 && condition1;
	bool down = mouse->button == 0 && mouse->action;

	if(over)
	{
		if(over && down)
		{
			if(m_state != MOUSE_DOWN) m_callback();
			m_state = MOUSE_DOWN;
			setColor(vec4(0.6, 0.6, 0.6, 1.0));
		}
		else
		{
			m_state = MOUSE_OVER;
			setColor(vec4(0.8, 0.8, 0.8, 1.0));
		}
	}
	else
	{
		m_state = MOUSE_NONE;
		setColor(vec4(1.0));
	}
}

void UIImageButton::render(const vec2& dimension)
{
	UIImage::render(dimension);
}
