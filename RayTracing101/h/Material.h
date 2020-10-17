#ifndef __MATERIAL_H_INCLUDED
#define __MATERIAL_H_INCLUDED

#include "h/Hitable.h"

class Material
{
public:
    virtual bool Scatter(const Ray &ray_in, const HitRecord &hit, vec3 &attenuation, Ray &ray_out) const = 0;
    virtual ~Material() = default;
};

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

class Lambertian : public Material
{
public:
    Lambertian(vec3 albedo) : m_albedo(albedo)
    {}

    bool Scatter(const Ray &ray_in, const HitRecord &hit, vec3 &attenuation, Ray &ray_out) const override
    {
        ray_out.origin = hit.pt;
        ray_out.direction = RandUnitVectorSemisphere(hit.nrm);

        attenuation = m_albedo;

        return true;
    }

private:
    vec3 m_albedo;
};

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

class Metal : public Material
{
public:
    Metal(vec3 albedo, real fuzziness) : m_albedo(albedo), m_fuzziness(Clamp01(fuzziness))
    {}

    bool Scatter(const Ray &ray_in, const HitRecord &hit, vec3 &attenuation, Ray &ray_out) const override
    {
        ray_out.origin = hit.pt;
        ray_out.direction = Reflect(ray_in.direction, hit.nrm);

        if (m_fuzziness > 0)
            ray_out.direction = (ray_out.direction + m_fuzziness * RandUnitVectorSemisphere(hit.nrm)).normalize();

        attenuation = m_albedo;

        return dot(ray_out.direction, hit.nrm) > 0;
    }

private:
    vec3 m_albedo;
    real m_fuzziness;
};

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

class Refractive : public Material
{
public:
    Refractive(real ior, vec3 attenuation = vec3(0.95)) 
        : m_ior(ior),
          m_attenuation(attenuation)
    {}

    bool Scatter(const Ray &ray_in, const HitRecord &hit, vec3 &attenuation, Ray &ray_out) const override
    {
        ray_out.origin = hit.pt;

        const real VdotN = dot(ray_in.direction, hit.nrm);

        const real probOfReflection = Schlick(abs(VdotN), m_ior);

        if (Rand01() > probOfReflection)
        {
            real ratio = VdotN < 0 ? 1 / m_ior : m_ior;
            ray_out.direction = Refract(ray_in.direction, hit.nrm, VdotN, ratio);
        }
        else
        {
            ray_out.direction = Reflect(ray_in.direction, hit.nrm, VdotN);
        }

        attenuation = m_attenuation;

        return true;
    }

private:
    real m_ior;
    vec3 m_attenuation;

    //-- probability of reflection
    real Schlick(real cosine, real ior) const
    {
        real r0 = (1 - ior) / (1 + ior);
        r0 *= r0;
        return r0 + (1 - r0)*pow(1 - cosine, 5);
    }
};

#endif