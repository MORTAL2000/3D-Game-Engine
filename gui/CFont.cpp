#include "CFont.h"

bool CFont::s_lib_loaded = false;
FT_Library CFont::s_ft_library;
Shader CFont::s_ft_shader;
bool CFont::s_shader_loaded = false;

CFont::CFont() : m_ft_loaded(false), m_color(1.0)
{
	//CFont::initialize();
}

CFont::~CFont()
{
	release();
}

bool CFont::initialize()
{
	if(s_lib_loaded) return true;
	if(FT_Init_FreeType(&s_ft_library))
	{
		Console::log("Failed to initialize Freetype font rendering");
		return false;
	}
	return (s_lib_loaded = true);
}

void CFont::release()
{
	if(CFont::s_lib_loaded)
	{
		for(int i = ' '; i <= '~'; i++)
		{
			glDeleteTextures(1, &(m_glyphTextures[i].texture));
		}
	}
}

bool CFont::loadFromFile(const string& filename, const uint16_t size)
{
	if(!CFont::s_lib_loaded) return false;

	if(!s_shader_loaded)
	{
		if(!s_ft_shader.load("shader/ttf.vsh", "shader/ttf.fsh"))
		{
			Console::log("Failed to load shader for font %s", filename.c_str());
			return false;
		}
		s_shader_loaded = true;
	}

	if(FT_New_Face(s_ft_library, filename.c_str(), 0, &m_face) != 0)
	{
		Console::log("Failed at loading font %s", filename.c_str());
		return false;
	}

	if(FT_Set_Char_Size(m_face, size << 6, size << 6, 96, 96) != 0)
	{
		Console::log("Failed at setting character size of font %s", filename.c_str());
		return false;
	}

	for(int i = ' '; i <= '~'; i++)
	{
		FT_Glyph glyph;
		uint32_t index = FT_Get_Char_Index(m_face, i);
		if(index == 0) continue;

		FT_Load_Glyph(m_face, index, FT_LOAD_TARGET_LIGHT);
		FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
		FT_Get_Glyph(m_face->glyph, &glyph);

		FT_GlyphSlot slot = m_face->glyph;
        FT_Bitmap& bitmap = slot->bitmap;
		uint32_t w = bitmap.width;
        uint32_t h = bitmap.rows;
		uint32_t texture = 0;

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		unsigned char* data = new unsigned char[w * h];
        memset(data, 0, w * h *  sizeof(unsigned char));
        memcpy(data, bitmap.buffer, sizeof(unsigned char) * w * h);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		delete[] data;

		CFont::Glyph render_glyph;
		render_glyph.texture = texture;
		render_glyph.dim = rect_t(w, h, slot->advance.x / 64, slot->metrics.horiBearingY / 64);
		m_glyphTextures[i] = render_glyph;

		FT_Done_Glyph(glyph);
	}

	FT_Done_Face(m_face);
	m_filename = filename;

	Console::log("Loaded font '%s'", Tokenizer::removePath(filename).c_str());

	return (m_ft_loaded = true);
}

rect_t CFont::renderf(const vec2& position, const vec2& viewport, const char* format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	return render(string(buffer), position, viewport);
}

rect_t CFont::render(const string& text, const vec2& position, const vec2& viewport)
{
	uint32_t vao = 0, vbo = 0, ibo = 0;
	rect_t size(position.x, position.y, 0, 0);
	if(text.empty())
	{
		return size;
	}

	if(!m_ft_loaded || !CFont::s_lib_loaded)
	{
		Console::log("Font context (%s) is invalid", m_filename.c_str());
		return size;
	}

	mat4 projectionMatrix = glm::ortho(0.0f, viewport.x, viewport.y, 0.0f, -1.0f, 1.0f);
	CFont::s_ft_shader.bind();
	CFont::s_ft_shader.mat4x4("projectionMatrix", projectionMatrix);
	CFont::s_ft_shader.vec4f("color", m_color);
	CFont::s_ft_shader.bind();

	uint16_t vlen = text.length() << 2;
    uint16_t ilen = text.length() * ((1 << 2) + 2);

    vertex2_t* verts = new vertex2_t[vlen];
    uint16_t* inds = new uint16_t[ilen];

    memset(inds, 0, sizeof(uint16_t) * ilen);
    memset(verts, 0, sizeof(vec2) * vlen);

    int max_w = 0, max_h = 0;
    int32_t last_w = position.x;
    float y = position.y;

	for(size_t i = 0; i < vlen; i += 4)
    {
        char c = text[i >> 2];

        char letter = (c > '~' || c < ' ') ? ' ' : c;
        rect_t dim = m_glyphTextures[letter].dim;

        float xw = last_w;
        float h = dim.y;
        last_w = xw + dim.w;

		if(c == '\n')
		{
			last_w = position.x;
			y += m_glyphTextures['H'].dim.y + m_glyphTextures['H'].dim.h;
		}

		if(c == '\t')
		{
			last_w += m_glyphTextures['H'].dim.w * 4;
		}

        verts[i].position = vec2(xw, y - dim.h);
        verts[i+1].position = vec2(last_w, y - dim.h);
        verts[i+2].position = vec2(last_w, y - dim.h + h);
        verts[i+3].position = vec2(xw, y - dim.h + h);

        verts[i].texcoord = vec2(0, 0);
        verts[i+1].texcoord = vec2(1, 0);
        verts[i+2].texcoord = vec2(1, 1);
        verts[i+3].texcoord = vec2(0, 1);

        int x = (i >> 2) * 6;
        inds[x] = i;
        inds[x+1] = i + 1;
        inds[x+2] = i + 3;
        inds[x+3] = i + 3;
        inds[x+4] = i + 2;
        inds[x+5] = i + 1;

        max_w += dim.w;
        max_h = (max_h > dim.h + h) ? max_h : dim.h + h;
    }
	size.w = max_w;
	size.h = max_h;

	CFont::s_ft_shader.bind();

	glGenBuffers(1, &vao);
	glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex2_t) * vlen, verts, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * ilen, inds, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex2_t), (void*)offsetof(struct vertex2_t, position));//VBO_OFFSET(0, vertex2_t, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex2_t), (void*)offsetof(struct vertex2_t, texcoord));//VBO_OFFSET(0, vertex2_t, texcoord));

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	for(size_t i = 0; i < text.length(); i++)
    {
        char c = (text[i] > '~' || text[i] < ' ') ? ' ' : text[i];

        glBindTexture(GL_TEXTURE_2D, m_glyphTextures[c].texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(sizeof(uint16_t) * i * 6));
    }

	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	CFont::s_ft_shader.unbind();

	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
	//glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);

	delete[] verts;
    delete[] inds;

	return size;
}
