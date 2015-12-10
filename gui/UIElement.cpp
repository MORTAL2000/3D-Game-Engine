/*
 * Copyright 2015 Alexander Koch
 * File: UIElement.cpp
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

#include "UIElement.h"

UIElement::UIElement() : m_id(-1), m_active(true) {}

void UIElement::setId(long id)
{
	m_id = id;
}

long UIElement::getId()
{
	return m_id;
}

void UIElement::setActive(bool active)
{
	m_active = active;
}

bool UIElement::isActive()
{
	return m_active;
}

void UIElement::processMouse(Mouse* mouse) {}
void UIElement::processScroll(double xOffset, double yOffset) {}
void UIElement::processKeyInput(int key, int scancode, int action, int mods) {}
void UIElement::processCharInput(unsigned int character) {}
void UIElement::render(const vec2& viewport) {}
