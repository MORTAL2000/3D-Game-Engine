#ifndef TEXTURE_LIBRARY_H
#define TEXTURE_LIBRARY_H

#include <vector>
#include <map>
#include <string>

#include "Texture2D.h"

typedef std::map<std::string, Texture2D> TextureCache;

class TextureLibrary
{
public:
	static TextureLibrary& getInstance();

	bool load(const std::string&, const std::string&);
	bool contains(const std::string&);
	Texture2D* get(const std::string&);
	void free();
private:
	TextureLibrary();
 	TextureCache m_cache;
	long m_texture_count;
};

#endif
