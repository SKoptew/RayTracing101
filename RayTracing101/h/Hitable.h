#ifndef __SURFACE_H_INCLUDED
#define __SURFACE_H_INCLUDED

#include <memory>
#include "h/Ray.h"
#include "AABoundBox.h"

class Material;
using MaterialSptr = std::shared_ptr<Material>;

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
    virtual AABoundBox GetAABoundBox() const = 0;

    virtual ~Hitable() = default;
};
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

class Sphere : public Hitable
{
public:
    Sphere(const vec3 &center, real radius, std::shared_ptr<Material> mat)
        : m_center(center),
          m_radius(radius),
          m_radiusInv(1/radius), 
          m_radius2(radius*radius),
          m_material(mat)
    {}

    bool Hit(const Ray &ray, real t_min, real t_max, HitRecord &hit) const override;
    AABoundBox GetAABoundBox() const override;

private:
    vec3         m_center;
    real         m_radius;
    real         m_radiusInv;
    real         m_radius2;
    MaterialSptr m_material;
};

#endif