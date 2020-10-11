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
        ray_out.origin    = hit.pt;
        ray_out.direction = (hit.nrm + RandUnitVector()).normalize();

        attenuation = m_albedo;

        return true;
    }

private:
    vec3 m_albedo;
};

class Metal : public Material
{
public:
    Metal(vec3 albedo, float fuzziness) : m_albedo(albedo), m_fuzziness(Clamp01(fuzziness))
    {}

    bool Scatter(const Ray &ray_in, const HitRecord &hit, vec3 &attenuation, Ray &ray_out) const override
    {
        ray_out.origin    = hit.pt;
        ray_out.direction = (Reflect(ray_in.direction, hit.nrm) + m_fuzziness * RandUnitVector()).normalize();

        attenuation = m_albedo;

        return dot(ray_out.direction, hit.nrm) > 0;
    }

private:
    vec3  m_albedo;
    float m_fuzziness;
};

#endif