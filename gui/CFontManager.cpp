#include "CFontManager.h"

std::map<std::string, std::shared_ptr<CFont>> CFontManager::m_fonts;

CFontManager::CFontManager()
{}

void CFontManager::initialize()
{
	CFont::initialize();
}

void CFontManager::loadFont(const std::string& key, const std::string& filename, const uint16_t size)
{
	std::shared_ptr<CFont> font(new CFont());
	if(font->loadFromFile(filename, size))
	{
		m_fonts[key] = font;
	}
}

CFont* CFontManager::getFont(const std::string& key)
{
	if(m_fonts.find(key) != m_fonts.end())
	{
		std::shared_ptr<CFont> font = m_fonts[key];
		font->setColor(vec4(1.0));
		return font.get();
	}
	return 0;
}
