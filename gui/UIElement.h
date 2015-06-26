#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include "../core/Commons.h"
#include "../core/Console.h"
#include "../core/Property.h"

typedef void (*Callback) ();

struct Mouse
{
	vec2 position;
	vec2 last_position;
	int action, button;

	bool isLeftDown()
	{
		return action && button == 0;
	}

	bool isRightDown()
	{
		return action && button == 1;
	}
};

class UIElement
{
public:
	UIElement();
	~UIElement();

	void setId(long);
	long getId();

	void setActive(bool);
	bool isActive();

	bool isNode() { return m_isNode; }

	virtual void processMouse(Mouse*);
	virtual void processScroll(double, double);
	virtual void processKeyInput(int, int, int, int);
	virtual void processCharInput(unsigned int);
	virtual void render(const vec2&);
protected:
	long m_id;
	bool m_active;
	bool m_isNode;
};

#endif
