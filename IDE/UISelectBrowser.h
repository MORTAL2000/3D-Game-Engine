#ifndef UI_SELECT_BROWSER_H
#define UI_SELECT_BROWSER_H

#include <vector>

#include "UIElement.h"
#include "UIImage.h"
#include "CFontManager.h"
#include "UIButton.h"

#include "../core/FileIO.h"

class UISelectBrowser : public UIElement
{
public:
	UISelectBrowser();
	~UISelectBrowser();

	void initialize(const Callback&);

	void setOption(const std::string&, const Callback&);

	void setPosition(const vec2&);
	vec2 getPosition();

	void processMouse(Mouse*);
	void processScroll(double, double);
	void render(const vec2&);

	std::string getSelection()
	{
		return m_path;
	}
private:
	Shader m_shader;
	Mesh m_mesh;
	vec2 m_position;
	vec2 m_scale;

	bool m_state;
	double m_yOffset;
	double m_scroll;

	std::vector<std::string> m_entries;
	std::string m_directory;

	std::string m_final;
	std::string m_path;

	long m_selection;

	UIButton m_option1;
	UIButton m_option2;
};

#endif
