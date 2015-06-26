#ifndef CFONT_H
#define CFONT_H
#include <string>
#include <map>
#include <cstddef>

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
	bool loadFromFile(const std::string& filename, const uint16_t size);

    void setColor(const vec4& color)
    {
        m_color = color;
    }

	/**
	 *	Renders a string using the font to the screen
	 */
     rect_t renderf(const vec2& position, const vec2& viewport, const char* format, ...);
     rect_t render(const std::string& text, const vec2& position, const vec2& viewport);

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

	std::map<char, Glyph> m_glyphTextures;
	FT_Face m_face;
	std::string m_filename;
};

#endif
