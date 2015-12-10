/*
 * Copyright 2015 Alexander Koch
 * File: CFontManager.cpp
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

#include "CFontManager.h"

std::map<string, std::shared_ptr<CFont>> CFontManager::m_fonts;

CFontManager::CFontManager()
{}

void CFontManager::initialize()
{
	CFont::initialize();
}

void CFontManager::loadFont(const string& key, const string& filename, const uint16_t size)
{
	std::shared_ptr<CFont> font(new CFont());
	if(font->loadFromFile(filename, size))
	{
		m_fonts[key] = font;
	}
}

CFont* CFontManager::getFont(const string& key)
{
	if(m_fonts.find(key) != m_fonts.end())
	{
		std::shared_ptr<CFont> font = m_fonts[key];
		font->setColor(vec4(1.0));
		return font.get();
	}
	return 0;
}
