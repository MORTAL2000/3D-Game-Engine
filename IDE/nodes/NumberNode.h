#ifndef NUMBER_NODE_H
#define NUMBER_NODE_H

#include "../UINode.h"
#include "../UIInput.h"

#include <string>

class NumberNode : public UINode
{
public:
	NumberNode();
	~NumberNode();

	void load();
	double getNumber();

	virtual void processMouse(Mouse*);
	virtual void processKeyInput(int, int, int, int);
	virtual void processCharInput(unsigned int);
	virtual void render(const vec2&);
private:
	UIInput m_input;

	double m_number;
};

#endif
