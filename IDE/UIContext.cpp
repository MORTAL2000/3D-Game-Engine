#include "UIContext.h"

std::map<long, UIElement*> UIContext::s_cache;
long UIContext::s_id;

UIContext::UIContext()
{

}

UIContext::~UIContext()
{

}

bool UIContext::initialize()
{
	int size = Property("size_ttf");
	CFontManager::initialize();
	CFontManager::loadFont("default", Property("font_ttf"), size);
	UIButton::initialize();
	UIImage::initialize();
	return true;
}

void UIContext::release()
{
	UIButton::release();
}

void UIContext::add(UIElement* element)
{
	if(element->getId() == -1)
	{
		long id = next();
		element->setId(id);
		s_cache[id] = element;
	}
}

void UIContext::remove(UIElement* element)
{
	if(element->getId() != -1)
	{
		s_cache.erase(element->getId());
		element->setId(-1);
	}
}

void UIContext::enableAll()
{
	for(std::map<long, UIElement*>::iterator it = s_cache.begin(); it != s_cache.end(); it++)
	{
		UIElement* element = it->second;
		element->setActive(true);
	}
}

void UIContext::disableAllExcept(UIElement* element)
{
	for(std::map<long, UIElement*>::iterator it = s_cache.begin(); it != s_cache.end(); it++)
	{
		UIElement* element_it = it->second;
		if(element_it->getId() != element->getId())
		{
			element_it->setActive(false);
		}
	}
}

UINode* UIContext::requestNodeConnection(const vec2& position, UINode* node, int input, int* index)
{
	for(std::map<long, UIElement*>::iterator it = s_cache.begin(); it != s_cache.end(); it++)
	{
		UIElement* element_it = it->second;
		if(element_it->isNode())
		{
			UINode* node0 = dynamic_cast<UINode*>(element_it);
			if(node0 == node) continue;
			if(node0)
			{
				UINode* temp = node0->connect(position, node, input, index);
				if(temp)
				{
					return temp;
				}
			}
		}
	}
	(*index) = -1;
	return 0;
}

void UIContext::removeActiveNode()
{
	for(std::map<long, UIElement*>::iterator it = s_cache.begin(); it != s_cache.end(); it++)
	{
		UIElement* element_it = it->second;
		if(element_it->isNode())
		{
			UINode* node = dynamic_cast<UINode*>(element_it);
			if(node)
			{
				if(node->isSelected())
				{
					remove(element_it);
				}
			}
		}
	}
}

void UIContext::processMouse(Mouse* mouse)
{
	for(std::map<long, UIElement*>::iterator it = s_cache.begin(); it != s_cache.end(); it++)
	{
		UIElement* element = it->second;
		if(element->isActive())
		{
			element->processMouse(mouse);
		}
	}
}

void UIContext::processScroll(double xOffset, double yOffset)
{
	for(std::map<long, UIElement*>::iterator it = s_cache.begin(); it != s_cache.end(); it++)
	{
		UIElement* element = it->second;
		if(element->isActive())
		{
			element->processScroll(xOffset, yOffset);
		}
	}
}

void UIContext::processKeyInput(int key, int scancode, int action, int mods)
{
	for(std::map<long, UIElement*>::iterator it = s_cache.begin(); it != s_cache.end(); it++)
	{
		UIElement* element = it->second;
		if(element->isActive())
		{
			element->processKeyInput(key, scancode, action, mods);
		}
	}
}

void UIContext::processCharInput(unsigned int ch)
{
	for(std::map<long, UIElement*>::iterator it = s_cache.begin(); it != s_cache.end(); it++)
	{
		UIElement* element = it->second;
		if(element->isActive())
		{
			element->processCharInput(ch);
		}
	}
}

void UIContext::render(const vec2& viewport)
{
	for(std::map<long, UIElement*>::iterator it = s_cache.begin(); it != s_cache.end(); it++)
	{
		it->second->render(viewport);
	}
}

long UIContext::next()
{
	return s_id++;
}
