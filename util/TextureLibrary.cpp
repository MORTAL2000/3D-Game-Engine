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

	m_tex2d_cache[key] = texture;
	m_texture_count++;
	return true;
}

bool TextureLibrary::load(const std::string& key, const std::string& xpos, const std::string& xneg, const std::string& ypos, const std::string& yneg, const std::string& zpos, const std::string& zneg)
{
	Cubemap texture;
	if(!texture.load(xpos, xneg, ypos, yneg, zpos, zneg)) return false;
	texture.setId(m_texture_count);

	m_cube_cache[key] = texture;
	m_texture_count++;
	return true;
}

bool TextureLibrary::containsTexture2D(const std::string& key)
{
	return m_tex2d_cache.find(key) != m_tex2d_cache.end();
}

bool TextureLibrary::containsCubemap(const std::string& key)
{
	return m_cube_cache.find(key) != m_cube_cache.end();
}

Texture2D* TextureLibrary::getTexture2D(const std::string& key)
{
	if(containsTexture2D(key))
	{
		return &m_tex2d_cache[key];
	}
	return 0;
}

Cubemap* TextureLibrary::getCubemap(const std::string& key)
{
	if(containsCubemap(key))
	{
		return &m_cube_cache[key];
	}
	return 0;
}

ITexture* TextureLibrary::get(const std::string& key)
{
	ITexture* tex = getTexture2D(key);
	if(!tex) tex = getCubemap(key);
	return tex;
}

void TextureLibrary::free()
{
	Console::log("Cache: Textures freed.");
	for(auto it = m_tex2d_cache.begin(); it != m_tex2d_cache.end(); it++)
	{
		it->second.clear();
	}
	for(auto it = m_cube_cache.begin(); it != m_cube_cache.end(); it++)
	{
		it->second.clear();
	}
}
