#ifndef UI_LIST_H
#define UI_LIST_H

#include <vector>

#include "UIElement.h"
#include "UIImage.h"
#include "CFontManager.h"

class UIList : public UIElement
{
public:
	struct Entry
	{
		bool isEmpty;
		std::string name;
		Callback callback;

		Entry() : isEmpty(true) {}
		Entry(const std::string& name) : name(name), isEmpty(false) {}
		Entry(const std::string& name, const Callback& callback) :
			name(name), callback(callback), isEmpty(false) {}
	};

	UIList();
	~UIList();

	void initialize();

	void setPosition(const vec2&);
	vec2 getPosition();

	void addOption(const Entry&);
	void addEmpty();

	void processMouse(Mouse*);
	void render(const vec2&);
private:
	UIImage m_image;
	UIImage m_highlight;
	vec2 m_position;
	bool m_drag;
	bool m_connect;
	vec2 m_char_dimension;
	int m_selection;
	double m_spacing;

	std::vector<Entry> m_entries;
};

#endif
