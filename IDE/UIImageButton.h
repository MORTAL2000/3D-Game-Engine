#ifndef UI_IMAGE_BUTTON_H
#define UI_IMAGE_BUTTON_H

#include "UIImage.h"

class UIImageButton : public UIImage
{
public:
	enum State
	{
		MOUSE_NONE,
		MOUSE_OVER,
		MOUSE_DOWN
	};

	UIImageButton();
	~UIImageButton();

	void setCallback(Callback);

	void processMouse(Mouse*);
	void render(const vec2&);
private:
	State m_state;
	Callback m_callback;
};

#endif
