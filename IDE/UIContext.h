#ifndef UI_CONTEXT_H
#define UI_CONTEXT_H

#include <vector>
#include <map>

#include "UIElement.h"
#include "CFontManager.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UINode.h"

#include "../core/Commons.h"
#include "../core/Property.h"

class UIContext
{
public:
	static bool initialize();
	static void release();

	static void add(UIElement*);
	static void remove(UIElement*);

	static void enableAll();
	static void disableAllExcept(UIElement*);

	static UINode* requestNodeConnection(const vec2&, UINode*, int, int*);
	static void removeActiveNode();

	static void processMouse(Mouse*);
	static void processScroll(double, double);
	static void processKeyInput(int, int, int, int);
	static void processCharInput(unsigned int);
	static void render(const vec2&);
private:
	UIContext();
	~UIContext();

	static std::map<long, UIElement*> s_cache;
	static long s_id;
	static long next();
};

#endif
