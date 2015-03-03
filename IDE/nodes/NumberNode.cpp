#include "NumberNode.h"
#include "../CFontManager.h"

NumberNode::NumberNode() : m_number(0)
{
	m_type = UINode::TYPE_NUMBER;
}

NumberNode::~NumberNode()
{

}

void NumberNode::load()
{
	initialize("Number");
	m_input.initialize();
}

double NumberNode::getNumber()
{
	std::string value = m_input.getValue();
	return (double)atof(value.c_str());
}

void NumberNode::processMouse(Mouse* mouse)
{
	UINode::processMouse(mouse);
}

void NumberNode::processKeyInput(int key, int scancode, int action, int mods)
{
	if(UINode::m_active) m_input.processKeyInput(key, scancode, action, mods);
}

void NumberNode::processCharInput(unsigned int ch)
{
	if(UINode::m_active) m_input.processCharInput(ch);
}

void NumberNode::render(const vec2& dimension)
{
	vec2 pos = m_position+m_scale*vec2(0.05, 0.5);
	m_input.setPosition(pos);
	UINode::render(dimension);
	m_input.render(dimension);

	CFont* font = CFontManager::getFont("default");
	font->setColor(vec4(0.1, 0.1, 0.1, 1.0));
	rect_t dim = font->getGlyphDimension('H');
	font->render(m_input.getValue(), pos + vec2(dim.w * 0.2, dim.h * 1.5), dimension);
	font->setColor(vec4(1.0));
}
