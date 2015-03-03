#ifndef UI_INPUT_H
#define UI_INPUT_H

#include <string>

#include "UIElement.h"
#include "UIImage.h"

class UIInput : public UIElement
{
public:
	UIInput();
	~UIInput();

	void initialize();

	void setPosition(const vec2&);
	void setScale(const vec2&);

	virtual void processKeyInput(int, int, int, int);
	virtual void processCharInput(unsigned int);
	virtual void render(const vec2&);

	std::string getValue();
private:
	UIImage m_image;
	vec2 m_scale;
	vec2 m_position;

	std::string m_value;
};

#endif
