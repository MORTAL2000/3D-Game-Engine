#ifndef ITEXTURE_H
#define ITEXTURE_H

class ITexture
{
public:
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void clear() = 0;
};

#endif
