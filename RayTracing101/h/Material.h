#ifndef __MATERIAL_H_INCLUDED
#define __MATERIAL_H_INCLUDED

#include "h/Hitable.h"

class Material
{
public:
    virtual bool Scatter(const Ray &ray_in, const HitRecord &hit, vec3 &attenuation, Ray &ray_out) const = 0;
    virtual ~Material() = default;
};

class Lambertian : public Material
{
public:
    Lambertian(vec3 albedo) : m_albedo(albedo)
    {}

    bool Scatter(const Ray &ray_in, const HitRecord &hit, vec3 &attenuation, Ray &ray_out) const override
    {
        ray_out = Ray(hit.pt, hit.nrm + RandUnitVector());
        attenuation = m_albedo;

        return true;
    }

private:
    vec3 m_albedo;
};

class Metal : public Material
{
public:
    Metal(vec3 albedo) : m_albedo(albedo)
    {}

    bool Scatter(const Ray &ray_in, const HitRecord &hit, vec3 &attenuation, Ray &ray_out) const override
    {
        ray_out = Ray(hit.pt, Reflect(ray_in.direction, hit.nrm));
        attenuation = m_albedo;

        return dot(ray_out.direction, hit.nrm) > 0;
    }

private:
    vec3 m_albedo;
};

#endif