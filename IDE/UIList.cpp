#include "UIList.h"
#include "UIContext.h"

UIList::UIList() : m_drag(false), m_selection(-1)
{

}

UIList::~UIList()
{

}

void UIList::initialize()
{
	m_spacing = 25;
	rect_t dimension = CFontManager::getFont("default")->getGlyphDimension('H');
	m_char_dimension = vec2(dimension.w, dimension.h);

	m_image.loadFromFile("resources/gui/list.png");
	m_image.setScale(vec2(220, 0));

	m_highlight.loadFromFile("resources/gui/selection.png");
	m_highlight.setScale(vec2(200, m_char_dimension.y+10));
	//m_highlight.setColor(vec4(0.9, 0.9, 0.9, 0.3));
	m_highlight.setColor(vec4(1.0));
}

void UIList::setPosition(const vec2& position)
{
	m_position = position;
	m_image.setPosition(position);
}

vec2 UIList::getPosition()
{
	return m_position;
}

void UIList::addOption(const Entry& entry)
{
	m_entries.push_back(entry);
}

void UIList::addEmpty()
{
	m_entries.push_back(Entry());
}

void UIList::processMouse(Mouse* mouse)
{
	vec2 position = mouse->position;
	vec2 scale = m_image.getScale();
	bool down = mouse->button == 0 && mouse->action;
	bool release = mouse->button == 0 && mouse->action == 0;

	bool condition0 = position.x > m_position.x && position.x < m_position.x + scale.x;
	bool condition1 = position.y > m_position.y && position.y < m_position.y + scale.y;
	bool over = condition0 && condition1;

	// 0xDEADC0DE
	// // draggable
	// if(m_drag && left_down)
	// {
	// 	m_position += mouse.position - mouse.last_position;
	// }
	// else if(m_drag && !left_down)
	// {
	// 	m_drag = false;
	// 	UIContext::enableAll();
	// }
	// else
	// {
	// 	m_drag = false;
	// }
	//
	// if(over && left_down)
	// {
	// 	m_drag = true;
	// 	UIContext::disableAllExcept(this);
	// }

	// connectable
	// if(m_connect && right_down)
	// {
	// 	// arrow.head = mouse.position;
	// }
	// else if(m_connect && !right_down)
	// {
	// 	m_connect = false;
	// 	// release and connect
	// 	// if(over node element)
	// 	// {
	// 	// 	bool connnection = other.requestDrop();
	// 	// 	if(connection)
	// 	// 	{
	// 	// 		this.connection = other;
	// 	// 	}
	// 	// }
	// }
	// else
	// {
	// 	m_connect = false;
	// }
	//
	// if(over && right_down)
	// {
	// 	m_connect = true;
	// }

	bool overAny = false;
	double yOffset = 25;
	for(size_t i = 0; i < m_entries.size(); i++)
	{
		if(m_entries[i].isEmpty)
		{
			yOffset -= m_spacing * 0.8;
			continue;
		}

		vec2 objPos = m_position + vec2(20, yOffset + i * m_spacing);
		bool condition0 = position.x > objPos.x && position.x < objPos.x + scale.x;
		bool condition1 = position.y > objPos.y && position.y < objPos.y + m_char_dimension.y + 10;
		bool over = condition0 && condition1;

		if(over)
		{
			overAny = true;
			m_selection = i;

			if(down)
			{
				m_entries[i].callback();
				UIContext::remove(this);
				UIContext::enableAll();
				break;
			}
		}
	}

	if(!overAny)
	{
		m_selection = -1;
	}

	if(!over && down)
	{
		UIContext::remove(this);
		UIContext::enableAll();
		return;
	}
}

void UIList::render(const vec2& viewport)
{
	double empty = 0;
	for(size_t i = 0; i < m_entries.size(); i++)
	{
		if(m_entries[i].isEmpty) empty++;
	}

	m_image.setScale(vec2(220, 30 - (m_spacing * 0.8 * empty) + (m_entries.size()+1) * m_spacing));
	m_image.setPosition(m_position);
	m_image.render(viewport);

	double yOffset = 40;
	for(size_t i = 0; i < m_entries.size(); i++)
	{
		if(m_entries[i].isEmpty)
		{
			yOffset -= m_spacing * 0.8;
			continue;
		}

		CFont* font = CFontManager::getFont("default");
		if(i == m_selection)
		{
			m_highlight.setPosition(m_position+vec2(10, (yOffset-10) + m_selection * m_spacing - 5));
			m_highlight.render(viewport);

			//font->setColor(vec4(0.2, 0.2, 0.2, 1.0));
		}

		font->render(m_entries[i].name, m_position+vec2(20, yOffset + i * m_spacing), viewport);

		/*if(i == m_selection)
		{
			font->setColor(vec4(1.0));
		}*/
	}
}
