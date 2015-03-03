#ifndef UI_NODE_H
#define UI_NODE_H

#include <map>
#include <vector>

#include "UIElement.h"
#include "UIImage.h"

class UINode;

class Connector
{
public:
	enum State
	{
		MOUSE_NONE,
		MOUSE_OVER,
		MOUSE_DOWN
	};

	Connector(const std::string& name) : m_color(0.3, 0.3, 0.3, 1.0), m_name(name), m_node(0) {}
	Connector() : m_node(0) {}

	std::string getName() { return m_name; }
	void setColor(const vec4& color) { m_color = color; }
	vec4 getColor() { return m_color; }

	void setConnection(UINode* node, int index)
	{
		m_node = node;
	 	m_index = index;
	}

	UINode* getConnection()
	{
		return m_node;
	}

	int getIndex()
	{
		return m_index;
	}

	vec2 start, end;
private:
	vec4 m_color;
	std::string m_name;
	UINode* m_node;
 	int m_index;
};

class UINode : public UIElement
{
public:
	enum Type
	{
		TYPE_NONE,
		TYPE_NUMBER,
		TYPE_VECTOR,
		TYPE_MESH_CUBE,
		TYPE_MESH_PLANE,
		TYPE_MESH_SPHERE,
		TYPE_MESH_ARRAY
	};

	UINode();
	~UINode();

	void initialize(const std::string&);

	void setPosition(const vec2&);
	vec2 getPosition();

	void addInput(const std::string&);
	void addOutput(const std::string&);

	UINode* getInputConnection(const std::string&);
	UINode* getInputConnection(int);
	UINode* connect(const vec2&, UINode*, int, int*);

	void freeInput(int);

	bool isSelected()
	{
		return m_active;
	}

	void setType(const Type&);
	Type getType();

	void processMouse(Mouse*);
	void processScroll(double, double);
	virtual void render(const vec2&);
protected:
	Mesh m_mesh;
	Mesh m_outline;
	Mesh m_line;
	Shader m_shader;

	std::string m_title;
	vec2 m_position;
	vec2 m_scale;
	bool m_active;
	bool m_drag;
	bool m_connect;

	long m_input_selection;
	long m_output_selection;

	std::vector<Connector> m_inputs;
	std::vector<Connector> m_outputs;

	Type m_type;
	float m_zoom;

	void renderQuad(const vec2&, const vec2&, const vec4&);
	void renderLine(const vec2&, const vec2&, const vec4&);
};

#endif
