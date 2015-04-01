#include "UISelectBrowser.h"

#include "UIContext.h"

UISelectBrowser::UISelectBrowser() : m_selection(-1), m_scale(1280-250, 720-50), m_yOffset(65), m_scroll(0)
{

}

UISelectBrowser::~UISelectBrowser()
{

}

void UISelectBrowser::initialize(const Callback& callback)
{
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
	m_mesh.load(vertices);

	m_shader.load("shader/ui.vsh", "shader/ui.fsh");
	m_shader.bind();
	m_shader.valuei("colorOnly", 1);
	m_shader.unbind();

	m_directory = FileIO::getCurrentDirectory();
	m_entries = FileIO::getFilesInDirectory(m_directory);
	m_entries.push_back("..");

	m_option1.setColor(UIButton::MOUSE_NONE, vec4(0.4, 0.4, 0.4, 1.0));
	m_option1.setColor(UIButton::MOUSE_OVER, vec4(0.2, 0.2, 0.2, 1.0));
	m_option1.setColor(UIButton::MOUSE_DOWN, vec4(0.3, 0.3, 0.3, 1.0));
	m_option2.setColor(UIButton::MOUSE_NONE, vec4(0.4, 0.4, 0.4, 1.0));
	m_option2.setColor(UIButton::MOUSE_OVER, vec4(0.2, 0.2, 0.2, 1.0));
	m_option2.setColor(UIButton::MOUSE_DOWN, vec4(0.3, 0.3, 0.3, 1.0));

	m_option2.setText("Cancel");
	m_option2.setCallback(callback);
}

void UISelectBrowser::setOption(const std::string& text, const Callback& callback)
{
	m_option1.setText(text);
	m_option1.setCallback(callback);
}

void UISelectBrowser::setPosition(const vec2& position)
{
	m_position = position;
}

vec2 UISelectBrowser::getPosition()
{
	return m_position;
}

void UISelectBrowser::processMouse(Mouse* mouse)
{
	m_option1.processMouse(mouse);
	m_option2.processMouse(mouse);

	vec2 position = mouse->position;
	bool down = mouse->button == 0 && mouse->action;

	bool overAny = false;
	for(size_t i = 0; i < m_entries.size(); i++)
	{
		vec2 objPos = m_position+vec2(0, i * 25 + m_yOffset + m_scroll);
		bool condition0 = position.x > objPos.x && position.x < objPos.x + m_scale.x;
		bool condition1 = position.y > objPos.y && position.y < objPos.y + 25;
		bool over = condition0 && condition1;

		if(over && !m_state)
		{
			overAny = true;
			m_selection = i;

			if(down)
			{
				std::string selection = m_entries[i];
				std::string path = m_directory + "/" + selection;
				if(FileIO::is_file(path.c_str()))
				{
					m_final = m_entries[i];
					m_path = path;
				}
				else
				{
					if(selection == "..")
			        {
						m_directory = m_directory.substr(0, m_directory.find_last_of("\\/"));
						m_entries = FileIO::getFilesInDirectory(m_directory);
						m_entries.push_back("..");
						m_state = true;
			        }
			        else
			        {
						m_directory = path;
			            m_entries = FileIO::getFilesInDirectory(m_directory);
						m_entries.push_back("..");
						m_state = true;
			        }
					m_scroll = 0;
				}
			}
		}
	}

	if(!down)
	{
		m_state = false;
	}

	if(!overAny)
	{
		m_selection = -1;
	}
}

void UISelectBrowser::processScroll(double xOffset, double yOffset)
{
	if(yOffset < 0)//if(yOffset < 0 && m_position.y +(m_entries.size()-1) * 25 + m_yOffset + m_scroll > m_scale.y)
	{
		m_scroll += yOffset * 8;
	}

	if(yOffset > 0)
	{
		m_scroll += yOffset * 8;
	}
}

void UISelectBrowser::render(const vec2& viewport)
{
	m_scale = viewport;//vec2(viewport.x - 250, viewport.y - 50);
	m_position = vec2(0.0);
	vec4 color = vec4(0.082, 0.0705, 0.066, 1.0);

	mat4 projectionMatrix = glm::ortho(0.0f, viewport.x, viewport.y, 0.0f, -1.0f, 1.0f);
	CFont* font = CFontManager::getFont("default");

	m_shader.bind();
	m_shader.mat4x4("projectionMatrix", projectionMatrix);
	m_shader.vec2f("scale", m_scale);
	m_shader.vec4f("color", color);
	m_shader.vec2f("offset", vec2(m_position.x, m_position.y));
	m_shader.unbind();
	m_mesh.render(&m_shader);

	// entries
	for(size_t i = 0; i < m_entries.size(); i++)
	{
		if(m_position.y + i * 25 + m_yOffset + m_scroll < m_position.y + 25) continue;

		vec4 mcolor = i % 2 == 0 ? vec4(0.2, 0.2, 0.2, 1.0) : vec4(0.15, 0.15, 0.15, 1.0);
		if(m_selection == i)
		{
			mcolor = vec4(0.2, 0.2, 0.8, 1.0);
		}

		m_shader.bind();
		m_shader.vec2f("scale", vec2(m_scale.x, 25));
		m_shader.vec4f("color", mcolor);
		m_shader.vec2f("offset", m_position+vec2(0, i * 25 + m_yOffset + m_scroll));
		m_shader.unbind();
		m_mesh.render(&m_shader);

		font->render(m_entries[i], m_position+vec2(30, 18 + i * 25 + m_yOffset + m_scroll), viewport);
	}

	m_shader.bind();
	m_shader.vec2f("scale", vec2(m_scale.x, 65));
	m_shader.vec4f("color", vec4(0.25, 0.25, 0.25, 1.0));
	m_shader.vec2f("offset", m_position);
	m_shader.unbind();
	m_mesh.render(&m_shader);

	// selection info
	m_shader.bind();
	m_shader.vec2f("scale", vec2(m_scale.x-120, 25));
	m_shader.vec4f("color", vec4(0.15, 0.15, 0.15, 1.0));
	m_shader.vec2f("offset", m_position+vec2(10, 5));
	m_shader.unbind();
	m_mesh.render(&m_shader);

	m_shader.bind();
	m_shader.vec2f("scale", vec2(m_scale.x-120, 25));
	m_shader.vec4f("color", vec4(0.15, 0.15, 0.15, 1.0));
	m_shader.vec2f("offset", m_position+vec2(10, 35));
	m_shader.unbind();
	m_mesh.render(&m_shader);

	m_option1.setPosition(m_position+vec2(m_scale.x-110, 2));
	m_option2.setPosition(m_position+vec2(m_scale.x-110, 32));

	m_option1.render(viewport);
	m_option2.render(viewport);

	font->render(m_directory, m_position+vec2(15, 20), viewport);
	font->render(m_final, m_position+vec2(15, 50), viewport);
}
