/*
 * Copyright 2015 Alexander Koch
 * File: UIElement.h
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include <core/Commons.h>
#include <core/Console.h>
#include <core/Property.h>

typedef void (*Callback) ();

struct Mouse
{
	vec2 position;
	vec2 last_position;
	int action, button;

	bool isLeftDown()
	{
		return action && button == 0;
	}

	bool isRightDown()
	{
		return action && button == 1;
	}
};

class UIElement
{
public:
	UIElement();

	void setId(long);
	long getId();

	void setActive(bool);
	bool isActive();

	bool isNode() { return m_isNode; }

	virtual void processMouse(Mouse*);
	virtual void processScroll(double, double);
	virtual void processKeyInput(int, int, int, int);
	virtual void processCharInput(unsigned int);
	virtual void render(const vec2&);
protected:
	long m_id;
	bool m_active;
	bool m_isNode;
};

#endif
