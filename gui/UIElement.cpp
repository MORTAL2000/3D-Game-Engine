#include "UIElement.h"

UIElement::UIElement() : m_id(-1), m_active(true)
{

}

UIElement::~UIElement()
{

}

void UIElement::setId(long id)
{
	m_id = id;
}

long UIElement::getId()
{
	return m_id;
}

void UIElement::setActive(bool active)
{
	m_active = active;
}

bool UIElement::isActive()
{
	return m_active;
}

void UIElement::processMouse(Mouse* mouse) {}
void UIElement::processScroll(double xOffset, double yOffset) {}
void UIElement::processKeyInput(int key, int scancode, int action, int mods) {}
void UIElement::processCharInput(unsigned int character) {}
void UIElement::render(const vec2& viewport) {}
