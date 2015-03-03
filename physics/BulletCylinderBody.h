#ifndef BULLET_CYLINDER_BODY_H
#define BULLET_CYLINDER_BODY_H

#include "BulletBody.h"

class BulletCylinderBody : public BulletBody
{
public:
    BulletCylinderBody();
    ~BulletCylinderBody();

    // half extent (x radius, y height, z radius)
    void setHalfExtent(const vec3&);
    vec3 getHalfExtent();

    void build(const quat&, const vec3&);
private:
    btCylinderShape* m_shape;
    vec3 m_half_extent;
};


#endif
