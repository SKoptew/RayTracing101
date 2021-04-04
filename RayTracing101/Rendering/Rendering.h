#ifndef __RENDERING_H_INCLUDED
#define __RENDERING_H_INCLUDED

#include "Geometry/Scene.h"
#include "Rendering/Camera.h"
#include "Utils/Image.h"

class Rendering
{
public:
    explicit Rendering(int samples_count = 256, int max_bounces = 32, real min_hit_distance = (real)0.0001);

    void Render(const Scene &scene, const Camera &camera, Image &image) const;

private:
    int  m_samples_count    = 256;
    int  m_max_bounces      = 32;
    real m_min_hit_distance = (real)0.0001;    

    vec3 CalcRayColor(const Ray &ray, const Scene &scene, real clip_far) const;
    vec3 MissShader  (const Ray &ray, vec3 attenuation) const;
};

#endif