#ifndef BULLET_BOX_BODY_H
#define BULLET_BOX_BODY_H

#include "BulletBody.h"

class BulletBoxBody : public BulletBody
{
public:
	BulletBoxBody();
	~BulletBoxBody();

	void setScale(const vec3&);
	vec3 getScale();

	void build(const quat&, const vec3&);
private:
	vec3 m_scale;
	btBoxShape* m_shape;
};

#endif
