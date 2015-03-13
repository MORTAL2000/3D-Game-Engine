#include "Cubemap.h"

Cubemap::Cubemap()
{

}

bool Cubemap::load(const std::string& xpos, const std::string& xneg, const std::string& ypos, const std::string& yneg, const std::string& zpos, const std::string& zneg)
{
	glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// HACK assuming that all textures have the same size, fix
	unsigned int width, height, bpp;
	unsigned char* xPos = TextureLoader::load(xpos, width, height, bpp);
	unsigned char* xNeg = TextureLoader::load(xneg, width, height, bpp);
	unsigned char* yPos = TextureLoader::load(ypos, width, height, bpp);
	unsigned char* yNeg = TextureLoader::load(yneg, width, height, bpp);
	unsigned char* zPos = TextureLoader::load(zpos, width, height, bpp);
	unsigned char* zNeg = TextureLoader::load(zneg, width, height, bpp);

	if(!xPos || !xNeg || !yPos || !yNeg || !zPos || !zNeg) return false;

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, xPos);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, xNeg);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, yPos);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, yNeg);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, zPos);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, bpp, GL_UNSIGNED_BYTE, zNeg);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	delete[] xPos;
	delete[] xNeg;
	delete[] yPos;
	delete[] yNeg;
	delete[] zPos;
	delete[] zNeg;
	return true;
}

void Cubemap::bind(GLenum unit)
{
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}

void Cubemap::unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);
}

void Cubemap::clear()
{
	glDeleteTextures(1, &m_texture);
}
