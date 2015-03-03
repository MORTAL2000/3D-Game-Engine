#include "TextureLibrary.h"

TextureLibrary::TextureLibrary() : m_texture_count(0)
{}

TextureLibrary& TextureLibrary::getInstance()
{
	static TextureLibrary instance;
	return instance;
}

bool TextureLibrary::load(const std::string& key, const std::string& path)
{
	Texture2D texture;
	if(!texture.load(path)) return false;

	texture.setId(m_texture_count);
	m_cache[key] = texture;

	m_texture_count++;
	return true;
}

bool TextureLibrary::contains(const std::string& key)
{
	return m_cache.find(key) != m_cache.end();
}

Texture2D* TextureLibrary::get(const std::string& key)
{
	if(contains(key))
	{
		return &m_cache[key];
	}
	return 0;
}

void TextureLibrary::free()
{
	for(auto it = m_cache.begin(); it != m_cache.end(); it++)
	{
		it->second.clear();
	}
}
