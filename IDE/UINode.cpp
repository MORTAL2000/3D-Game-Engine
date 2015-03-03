#include "UINode.h"
#include "UIContext.h"

UINode::UINode() : m_drag(false), m_scale(1.0), m_zoom(1)
{
	UIElement::m_isNode = true;
}

UINode::~UINode()
{

}

void UINode::initialize(const std::string& title)
{
	m_title = title;

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

	vertices.clear();
	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v3);
	vertices.push_back(v0);
	m_outline.load(vertices);

	m_shader.load("shader/ui.vsh", "shader/ui.fsh");
	m_shader.bind();
	m_shader.valuei("colorOnly", 1);
	m_shader.unbind();
}

void UINode::setPosition(const vec2& position)
{
	m_position = position;
}

vec2 UINode::getPosition()
{
	return m_position;
}

void UINode::addInput(const std::string& input)
{
	m_inputs.push_back(Connector(input));
}

void UINode::addOutput(const std::string& output)
{
	m_outputs.push_back(Connector(output));
}

UINode* UINode::getInputConnection(const std::string& key)
{
	for(size_t i = 0; i < m_inputs.size(); i++)
	{
		if(m_inputs[i].getName() == key)
		{
			return m_inputs[i].getConnection();
		}
	}
	return 0;
}

UINode* UINode::getInputConnection(int index)
{
	if(index >= 0 && index < m_inputs.size())
	{
		return m_inputs[index].getConnection();
	}
	return 0;
}

UINode* UINode::connect(const vec2& position, UINode* node, int input, int* index)
{
	for(size_t i = 0; i < m_inputs.size(); i++)
	{
		vec2 pos = m_position-vec2(10, 0.0) + vec2(0.0, 25+i*20);
		vec2 scale = vec2(10);

		bool condition0 = position.x > pos.x && position.x < pos.x + scale.x;
		bool condition1 = position.y > pos.y && position.y < pos.y + scale.y;
		bool over = condition0 && condition1;

		if(over)
		{
			m_inputs[i].setConnection(node, input);
			(*index) = i;
			return this;
		}
	}
	(*index) = -1;
	return 0;
}

void UINode::freeInput(int index)
{
	m_inputs[index].setConnection(0, 0);
}

void UINode::setType(const UINode::Type& type)
{
	m_type = type;
}

UINode::Type UINode::getType()
{
	return m_type;
}

void UINode::processMouse(Mouse* mouse)
{
	vec2 position = mouse->position;
	bool left_down = mouse->button == 0 && mouse->action;
	bool release = mouse->button == 0 && mouse->action == 0;

	bool condition0 = position.x > m_position.x && position.x < m_position.x + m_scale.x;
	bool condition1 = position.y > m_position.y && position.y < m_position.y + m_scale.y;
	bool over = condition0 && condition1;

	// draggable
	if(m_drag && left_down)
	{
		m_position += mouse->position - mouse->last_position;
	}
	else if(m_drag && !left_down)
	{
		m_drag = false;
		UIContext::enableAll();
	}
	else
	{
		m_drag = false;
	}

	if(over && left_down)
	{
		m_active = true;
		m_drag = true;
		UIContext::disableAllExcept(this);
	}

	if(!over && left_down)
	{
		m_active = false;
	}

	bool right_down = mouse->button == 1 && mouse->action;

	for(size_t i = 0; i < m_outputs.size(); i++)
	{
		vec2 pos = m_position+vec2(m_scale.x, 25+i*20);
		vec2 scale = vec2(10);

		condition0 = position.x > pos.x && position.x < pos.x + scale.x;
		condition1 = position.y > pos.y && position.y < pos.y + scale.y;
		over = condition0 && condition1;

		m_outputs[i].start = pos+vec2(5);

		if(over)
		{
			m_outputs[i].setColor(vec4(0.2, 0.2, 0.2, 1.0));
		}
		else
		{
			m_outputs[i].setColor(vec4(0.3, 0.3, 0.3, 1.0));
		}

		if(m_output_selection == i && right_down)
		{
			UINode* cnode = m_outputs[i].getConnection();
			if(cnode)
			{
				int cindex = m_outputs[i].getIndex();
				cnode->freeInput(cindex);
			}

			m_outputs[i].setConnection(0, 0);
			m_outputs[i].end = position;
		}
		else if(m_output_selection == i && !right_down)
		{
			m_output_selection = -1;

			int index = -1;
		  	UINode* other = UIContext::requestNodeConnection(position, this, i, &index);
			if(other)
			{
				m_outputs[i].setConnection(other, index);
			}
			else
			{
				m_outputs[i].end = vec2(0.0);
			}
		}

		if(over && right_down)
		{
			m_output_selection = i;
		}
	}
}

void UINode::processScroll(double xOffset, double yOffset)
{
	m_zoom += yOffset * 5;
}

void UINode::renderQuad(const vec2& position, const vec2& size, const vec4& color)
{
	m_shader.bind();
	m_shader.vec2f("scale", size);
	m_shader.vec4f("color", color);
	m_shader.vec2f("offset", position);
	m_shader.unbind();
	m_mesh.render(&m_shader);

	m_shader.bind();
	m_shader.vec4f("color", m_active ? vec4(0.8, 0.8, 0.8, 1.0) : vec4(0.1, 0.1, 0.1, 1.0));
	m_shader.unbind();
	m_outline.render(&m_shader, GL_LINES, 8);
}

void UINode::renderLine(const vec2& start, const vec2& end, const vec4& color)
{
	std::vector<vec3> vertices;
	vertices.push_back(vec3(start, 0.0));
	vertices.push_back(vec3(end, 0.0));
	m_line.load(vertices);

	m_shader.bind();
	m_shader.vec2f("scale", vec2(1.0));
	m_shader.vec4f("color", color);
	m_shader.vec2f("offset", vec2(0.0));
	m_shader.unbind();

	m_line.render(&m_shader, GL_LINES, 2);
}

void UINode::render(const vec2& viewport)
{
	int in = m_inputs.size();
	int out = m_outputs.size();
	double sizeY = (((in > out) ? in : out)+1) * 30;
	m_scale = vec2(120, sizeY);

	mat4 projectionMatrix = glm::ortho(0.0f, viewport.x, viewport.y, 0.0f, -1.0f, 1.0f);
	m_shader.bind();
	m_shader.mat4x4("projectionMatrix", projectionMatrix);
	m_shader.unbind();

	vec4 color0 = vec4(0.2, 0.2, 0.8, 1.0);
	vec4 color1 = vec4(0.25, 0.25, 0.25, 1.0);

	CFont* font = CFontManager::getFont("default");
	int w = font->getGlyphDimension('H').w;

	renderQuad(m_position, vec2(m_scale.x, 20), color0);
	renderQuad(m_position+vec2(0.0f, 20), vec2(m_scale.x, m_scale.y-20), color1);

	for(size_t i = 0; i < m_inputs.size(); i++)
	{
		std::string name = m_inputs[i].getName();
		vec4 color = m_inputs[i].getColor();
		font->render(name, m_position+vec2(w, 35+i*20), viewport);
		renderQuad(m_position-vec2(10, 0.0) + vec2(0.0, 25+i*20), vec2(10), color);
	}

	for(size_t i = 0; i < m_outputs.size(); i++)
	{
		std::string name = m_outputs[i].getName();
		vec4 color = m_outputs[i].getColor();

		font->render(name, m_position+vec2(m_scale.x-w*(name.size()+1), 35+i*20), viewport);
		renderQuad(m_position+vec2(m_scale.x, 25+i*20), vec2(10), color);

		if(m_outputs[i].end != vec2(0.0))
		{
			UINode* cnode = m_outputs[i].getConnection();
			if(cnode)
			{
				vec2 pos2 = cnode->getPosition()-vec2(10, 0.0) + vec2(0.0, 25+m_outputs[i].getIndex()*20)+vec2(5, 5);
				m_outputs[i].end = pos2;
			}

			renderLine(m_outputs[i].start, m_outputs[i].end, vec4(0.2, 0.2, 0.8, 1.0));
		}
	}

	font->render(m_title, m_position+m_scale*vec2(0.5, 0.0)-vec2(w * m_title.size()*0.5, 0.0)+vec2(0.0, 15), viewport);
}
