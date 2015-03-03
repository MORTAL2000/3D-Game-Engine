#include "UIButton.h"

#include "CFontManager.h"

Texture2D UIButton::s_texture;
bool UIButton::s_loaded = false;
Shader UIButton::s_shader;
bool UIButton::s_shader_loaded;
Mesh UIButton::s_mesh;

UIButton::UIButton() : m_color_normal(1.0), m_color_over(1.0),
	m_color_down(1.0), m_state(MOUSE_NONE)
{}

UIButton::~UIButton()
{

}

bool UIButton::initialize()
{
	if(s_loaded) return true;
	UIButton::s_texture.setFiltering(GL_LINEAR);
	if(!UIButton::s_texture.load(Property("button_gui"), false))
	{
		Console::log("Failed to load UIButton skin");
		return false;
	}

	return (s_loaded = true);
}

void UIButton::release()
{
	if(s_loaded)
	{
		s_texture.clear();
	}
}

void UIButton::setPosition(const vec2& position)
{
	m_position = position;
}

vec2 UIButton::getPosition()
{
	return m_position;
}

void UIButton::setColor(const State& state, const vec4& color)
{
	switch(state)
	{
		case MOUSE_NONE:
		{
			m_color_normal = color;
			break;
		}
		case MOUSE_OVER:
		{
			m_color_over = color;
			break;
		}
		case MOUSE_DOWN:
		{
			m_color_down = color;
			break;
		}
		default:
		{
			m_color_normal = color;
		}
	}
}

vec4 UIButton::getColor(const State& state)
{
	switch(state)
	{
		case MOUSE_NONE:
		{
			return m_color_normal;
		}
		case MOUSE_OVER:
		{
			return m_color_over;
		}
		case MOUSE_DOWN:
		{
			return m_color_down;
		}
		default:
		{
			return m_color_normal;
		}
	}
}

void UIButton::setText(const std::string& text)
{
	if(!s_shader_loaded)
	{
		if(!UIButton::s_shader.load("shader/ui.vsh", "shader/ui.fsh"))
		{
			Console::log("Could not load UIButton shader");
		}
		else
		{
			s_shader_loaded = true;
			std::vector<vec3> vertices;
			vec3 v0 = vec3(0,0,0);
			vec3 v1 = vec3(1,0,0);
			vec3 v2 = vec3(1,1,0);
			vec3 v3 = vec3(0,1,0);
			vertices.push_back(v0);
			vertices.push_back(v1);
			vertices.push_back(v2);
			vertices.push_back(v2);
			vertices.push_back(v3);
			vertices.push_back(v0);
			s_mesh.load(vertices);
		}
	}

	m_text = text;
}

std::string UIButton::getText()
{
	return m_text;
}

void UIButton::setCallback(Callback callback)
{
	m_callback = callback;
}

void UIButton::processMouse(Mouse* mouse)
{
	vec2 position = mouse->position;
	bool condition0 = position.x > m_position.x && position.x < m_position.x + 90;
	bool condition1 = position.y > m_position.y && position.y < m_position.y + 30;
	bool over = condition0 && condition1;
	bool down = mouse->button == 0 && mouse->action;

	if(over)
	{
		if(over && down)
		{
			if(m_state != MOUSE_DOWN) m_callback();
			m_state = MOUSE_DOWN;
		}
		else
		{
			m_state = MOUSE_OVER;
		}
	}
	else
	{
		m_state = MOUSE_NONE;
	}
}

void UIButton::render(const vec2& viewport)
{
	if(m_text.empty() || !UIButton::s_shader_loaded || !UIButton::s_loaded) return;

	vec2 scale = vec2(90, 30);
	vec4 color = getColor(m_state);

	mat4 projectionMatrix = glm::ortho(0.0f, viewport.x, viewport.y, 0.0f, -1.0f, 1.0f);
	UIButton::s_shader.bind();
	UIButton::s_shader.mat4x4("projectionMatrix", projectionMatrix);
	UIButton::s_shader.vec2f("scale", scale);
	UIButton::s_shader.vec4f("color", color);
	UIButton::s_shader.vec2f("offset", vec2(m_position.x, m_position.y));
	UIButton::s_shader.unbind();

	UIButton::s_texture.bind(GL_TEXTURE0);
	UIButton::s_mesh.render(&UIButton::s_shader);
	UIButton::s_texture.unbind();

	CFont* font = CFontManager::getFont("default");
	font->render(m_text, m_position+vec2(20,20), viewport);
}
