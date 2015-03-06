#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "../core/Commons.h"

class ITexture
{
public:
	ITexture() : m_id(0) {}
	virtual void bind(GLenum) {}
	virtual void unbind() {}
	virtual void clear() {}

	void setId(long id) { m_id = id; }
	long getId() { return m_id; }
protected:
	long m_id;
};

#endif
