#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <string>

#include "UIElement.h"
#include "CFont.h"
#include "Types.h"
#include "../rendering/Mesh.h"
#include "../core/Property.h"

class UIButton : public UIElement
{
public:
	enum State
	{
		MOUSE_NONE,
		MOUSE_OVER,
		MOUSE_DOWN
	};

	UIButton();
	~UIButton();

	static bool initialize();
	static void release();

	void setPosition(const vec2&);
	vec2 getPosition();

	void setColor(const State&, const vec4&);
	vec4 getColor(const State&);

	void setText(const std::string&);
	std::string getText();

	void setCallback(Callback);

	void processMouse(Mouse*);
	void render(const vec2&);
private:
	static Texture2D s_texture;
	static bool s_loaded;
	static Shader s_shader;
	static bool s_shader_loaded;
	static Mesh s_mesh;

	vec2 m_position;

	State m_state;
	vec4 m_color_normal;
	vec4 m_color_over;
	vec4 m_color_down;

	std::string m_text;
	Callback m_callback;
};

#endif
