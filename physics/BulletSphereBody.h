#ifndef BULLET_SPHERE_BODY_H
#define BULLET_SPHERE_BODY_H

#include "BulletBody.h"

class BulletSphereBody : public BulletBody
{
public:
	BulletSphereBody();
	~BulletSphereBody();

	void setRadius(double);
	double getRadius();

	void build(const quat&, const vec3&);
private:
	double m_radius;
	btSphereShape* m_shape;
};

#endif
