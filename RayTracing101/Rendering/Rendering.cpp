#include "Rendering/Rendering.h"
#include "Rendering/Material.h"

#include <iostream>

//#define DBG_RENDER_BOUNCES_COUNT
//#define DBG_SHOW_BOUNCES_OVERRUN
//#define CHECK_NAN

Rendering::Rendering(int samples_count, int max_bounces, real min_hit_distance)
    : m_samples_count   (samples_count),
      m_max_bounces     (max_bounces),
      m_min_hit_distance(min_hit_distance)
{
}

void Rendering::Render(const Scene &scene, const Camera &camera, Image &image) const
{
    const int  w     = image.Width();
    const int  h     = image.Height();
    const real inv_w = 1 / real(w - 1);
    const real inv_h = 1 / real(h - 1);

    auto data = image.GetData();

    const real clip_far = camera.ClipFar();

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            int samples_ok = 0;
            vec3 color = { 0,0,0 };
            for (int sample_num = 0; sample_num < m_samples_count; ++sample_num)
            {
                const real u = (x + (Rand01()*real(0.5) - 1)) * inv_w;
                const real v = (y + (Rand01()*real(0.5) - 1)) * inv_h;

                const vec3 sample_color = CalcRayColor(camera.GetRay(u, v), scene, clip_far);

#ifdef CHECK_NAN
                if (!sample_color.isNaN())
#endif
                {
                    color += sample_color;
                    ++samples_ok;
                }
            }
            color /= real(samples_ok);

#ifndef DBG_RENDER_BOUNCES_COUNT
            color = LinearToSrgb(color);
#endif
            image.SetPixel(x, y, color);
        }

        std::cerr << "\rProcessed: " << y * 100 / h << "%..." << std::flush;
    }

    std::cerr << "\nDone\n";
}

vec3 Rendering::CalcRayColor(const Ray &_ray, const Scene &scene, real clip_far) const
{
    vec3 acc_attenuation = vec3(1);

    Ray ray = _ray;
    Ray ray_scattered;
    HitRecord hit;
    vec3 attenuation;

    int bounce = 0;
    for (; bounce < m_max_bounces; ++bounce)
    {
        if (scene.Hit(ray, m_min_hit_distance, bounce == 0 ? clip_far : real(FLT_MAX), hit))
        {
            if (hit.mat->Scatter(ray, hit, attenuation, ray_scattered)) // ray scattered
            {
                acc_attenuation *= attenuation;
                ray = ray_scattered;
                ray.inv_direction = vec3(1 / ray.direction.x, 1 / ray.direction.y, 1 / ray.direction.z);
            }
            else // ray hit emissive surface or absorbed 
            {
                return hit.mat->Emitted(hit.pt) * acc_attenuation;
            }
        }
        else
            break;
    }

    // ray missed

#ifdef DBG_SHOW_BOUNCES_OVERRUN
    if (bounce == MAX_BOUNCES)
        return vec3(1, 0, 1);
#endif

#ifdef DBG_RENDER_BOUNCES_COUNT
    return vec3(real(bounce) / MAX_BOUNCES);
#endif

    return MissShader(ray, acc_attenuation);
}

vec3 Rendering::MissShader(const Ray &ray, vec3 attenuation) const
{
    const real t = 0.5f * (ray.direction.getNormalized().y + 1.f);
    const vec3 sky_color = lerp(vec3(1, 1, 1), vec3(0.5f, 0.7f, 1.f), t);

    return sky_color * attenuation;
}