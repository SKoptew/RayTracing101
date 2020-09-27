#ifndef __SURFACE_H_INCLUDED
#define __SURFACE_H_INCLUDED

#include "h/Ray.h"

struct HitRecord
{
    real t;
    vec3 pt;
    vec3 nrm;
};

class Hitable
{
public:
    virtual bool Hit(const Ray &ray, real t_min, real t_max, HitRecord &hit) const = 0;
};
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

class Sphere : public Hitable
{
public:
    Sphere(const vec3 &center, real radius) : m_center(center), m_radiusInv(1/radius), m_radius2(radius*radius) {}

    bool Hit(const Ray &ray, real t_min, real t_max, HitRecord &hit) const override;

private:
    vec3 m_center;
    real m_radiusInv;
    real m_radius2;
};

#endif