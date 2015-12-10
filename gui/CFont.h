/*
 * Copyright 2015 Alexander Koch
 * File: CFont.h
 * Description: Class for font rendering.
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

#ifndef CFONT_H
#define CFONT_H
#include <string>
#include <map>
#include <cstddef>

using std::string;
using std::map;

#include <core/Commons.h>
#include <util/Shader.h>
#include <util/Texture2D.h>

#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>
#include <ftoutln.h>
#include <fttrigon.h>

#include "Types.h"
#include "UIElement.h"

//#define VBO_OFFSET(count, vertex, field) \
//    (void*)((count * sizeof(vertex)) + (long int)&(((vertex*)0)->field))

class CFont
{
public:
	CFont();
	~CFont();

	/**
	 *	Initializes freetype font
	 */
	static bool initialize();

	/**
	 *	Released freetype
	 */
	void release();

	/**
	 *	Loads a font from file
	 *  @param filename File to load (*.ttf)
	 *  @param size Size of the font
	 */
	bool loadFromFile(const string& filename, const uint16_t size);

    void setColor(const vec4& color)
    {
        m_color = color;
    }

	/**
	 *	Renders a string using the font to the screen
	 */
	rect_t renderf(const vec2& position, const vec2& viewport, const char* format, ...);
	rect_t render(const string& text, const vec2& position, const vec2& viewport);

	/**
	 *	Get the width of a string
	 */
	int getStringWidth(const string& text)
	{
		int sz = 0;
		for(int i = 0; i < text.length(); i++)
		{
			sz += m_glyphTextures[text[i]].dim.w;
		}
		return sz;
	}

	/**
	 *	Get the height of a string
	 */
	int getStringHeight(const string& text)
	{
		int sz = 0;
		for(int i = 0; i < text.length(); i++)
		{
			int tmp = m_glyphTextures[text[i]].dim.h;
			if(tmp > sz) sz = tmp;
		}
		return sz;
	}

	rect_t getGlyphDimension(char ch)
    {
 		return m_glyphTextures[ch].dim;
    }
private:
	static FT_Library s_ft_library;
	static bool s_lib_loaded;
    static Shader s_ft_shader;
    static bool s_shader_loaded;
	bool m_ft_loaded;
    vec4 m_color;

	struct Glyph
	{
		uint32_t texture;
		rect_t dim;
	};

	map<char, Glyph> m_glyphTextures;
	FT_Face m_face;
	string m_filename;
};

#endif
