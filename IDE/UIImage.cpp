#include "UIImage.h"

Shader UIImage::s_shader;
Mesh UIImage::s_mesh;
bool UIImage::s_loaded;

UIImage::UIImage() : m_color(1.0), m_position(0.0), m_scale(1.0)
{

}

UIImage::~UIImage()
{
	if(m_texture.isValid())
	{
		m_texture.clear();
	}
}

bool UIImage::initialize()
{
	if(s_loaded) return true;

	if(!s_shader.load("shader/ui.vsh", "shader/ui.fsh"))
	{
		Console::log("Failed to initialize UIImage shader");
		return false;
	}

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

	return (s_loaded = true);
}

void UIImage::setPosition(const vec2& position)
{
	m_position = position;
}

vec2 UIImage::getPosition()
{
	return m_position;
}

void UIImage::setScale(const vec2& scale)
{
	m_scale = scale;
}
vec2 UIImage::getScale()
{
	return m_scale;
}

void UIImage::setColor(const vec4& color)
{
	m_color = color;
}

vec4 UIImage::getColor()
{
	return m_color;
}

bool UIImage::loadFromFile(const std::string& filename)
{
	if(!m_texture.load(filename, false))
	{
		Console::log("Failed to load texture (%s)", filename.c_str());
		return false;
	}
	return true;
}

void UIImage::render(const vec2& viewport)
{
	mat4 projectionMatrix = glm::ortho(0.0f, viewport.x, viewport.y, 0.0f, -1.0f, 1.0f);
	UIImage::s_shader.bind();
	UIImage::s_shader.mat4x4("projectionMatrix", projectionMatrix);
	UIImage::s_shader.vec2f("scale", m_scale);
	UIImage::s_shader.vec4f("color", m_color);
	UIImage::s_shader.vec2f("offset", vec2(m_position.x, m_position.y));
	UIImage::s_shader.unbind();

	m_texture.bind(GL_TEXTURE0);
	UIImage::s_mesh.render(&UIImage::s_shader);
	m_texture.unbind();
}
