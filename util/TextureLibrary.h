#ifndef TEXTURE_LIBRARY_H
#define TEXTURE_LIBRARY_H

#include <vector>
#include <map>
#include <string>

#include "ITexture.h"
#include "Texture2D.h"
#include "Cubemap.h"

typedef std::map<std::string, Texture2D> Texture2DCache;
typedef std::map<std::string, Cubemap> CubemapCache;

class TextureLibrary
{
public:
	static TextureLibrary& getInstance();

	bool load(const std::string&, const std::string&);
	bool load(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

	bool containsTexture2D(const std::string&);
	bool containsCubemap(const std::string&);
	Texture2D* getTexture2D(const std::string&);
	Cubemap* getCubemap(const std::string&);
	ITexture* get(const std::string&);

	void free();
private:
	TextureLibrary();
 	Texture2DCache m_tex2d_cache;
	CubemapCache m_cube_cache;
	long m_texture_count;
};

#endif
