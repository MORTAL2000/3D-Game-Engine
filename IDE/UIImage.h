#ifndef UI_IMAGE_H
#define UI_IMAGE_H

#include <string>

#include "UIElement.h"
#include "../rendering/Mesh.h"
#include "../util/Shader.h"
#include "../util/Texture2D.h"

class UIImage : public UIElement
{
public:
	UIImage();
	~UIImage();

 	static bool initialize();

	void setPosition(const vec2&);
	vec2 getPosition();

	void setScale(const vec2&);
	vec2 getScale();

	void setColor(const vec4&);
	vec4 getColor();

	bool loadFromFile(const std::string&);
	virtual void render(const vec2&);
protected:
	static Shader s_shader;
	static Mesh s_mesh;
	static bool s_loaded;

	Texture2D m_texture;
	vec4 m_color;
	vec2 m_position;
	vec2 m_scale;
};

#endif
