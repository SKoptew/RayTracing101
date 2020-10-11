#ifndef __SURFACE_H_INCLUDED
#define __SURFACE_H_INCLUDED

#include "h/Ray.h"

class Material;

struct HitRecord
{
    real t;
    vec3 pt;
    vec3 nrm;
    Material *mat;
};

class Hitable
{
public:
    virtual bool Hit(const Ray &ray, real t_min, real t_max, HitRecord &hit) const = 0;

    Hitable(Material* material) : m_material(material)
    {}

    virtual ~Hitable() = default;

protected:
    Material* m_material;
};
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

class Sphere : public Hitable
{
public:
    Sphere(const vec3 &center, real radius, Material* mat) 
        : Hitable(mat),
          m_center(center), 
          m_radiusInv(1/radius), 
          m_radius2(radius*radius) 
    {}

    bool Hit(const Ray &ray, real t_min, real t_max, HitRecord &hit) const override;

private:
    vec3 m_center;
    real m_radiusInv;
    real m_radius2;
};

#endif