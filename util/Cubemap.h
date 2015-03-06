#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include "ITexture.h"
#include "TextureLoader.h"

class Cubemap : public ITexture
{
public:
	Cubemap();

 	bool load(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
	void bind(GLenum);
	void unbind();
	void clear();

private:
	unsigned int m_texture;
};

/*http://www.codinglabs.net/article_physically_based_rendering.aspx*/
/*http://aerotwist.com/tutorials/create-your-own-environment-maps/*/

#endif
