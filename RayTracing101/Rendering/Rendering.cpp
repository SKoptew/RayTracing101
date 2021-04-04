#include "Rendering/Rendering.h"
#include "Rendering/Material.h"

#include <iostream>
#include <thread>

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
    RenderRows(scene, camera, image, 0, image.Height());
    std::cerr << "\nDone\n";
}

void Rendering::RenderMT(const Scene &scene, const Camera &camera, Image &image) const
{
    const unsigned int numthreads = std::thread::hardware_concurrency();

    const int h = image.Height();    
    const int block_height = h / numthreads;

    std::vector<std::thread> threads(numthreads-1);

    for (unsigned int i = 0; i < numthreads-1; ++i)
    {
        const int start_row = i * block_height;
        const int end_row   = start_row + block_height;

        threads[i] = std::thread(&Rendering::RenderRows, *this, std::cref(scene), std::cref(camera), std::ref(image), start_row, end_row);
    }

    RenderRows(scene, camera, image, (numthreads-1) * block_height, h);

    for (auto& entry : threads)
        entry.join();

    std::cerr << "\nDone in " << numthreads << " threads\n";
}

void Rendering::RenderRows(const Scene &scene, const Camera &camera, Image &image, int start_row, int end_row) const
{
    const int  w        = image.Width();
    const int  h        = image.Height();
    const real inv_w    = 1 / real(w - 1);
    const real inv_h    = 1 / real(h - 1);
    const real clip_far = camera.ClipFar();

    for (int y = start_row; y < end_row; ++y)
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

        if (end_row == h) // only main thread writes to console
            std::cerr << "\rProcessed: " << (y- start_row) * 100 / (end_row - start_row) << "%..." << std::flush;
    }
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
    return vec3(0);

    const real t = 0.5f * (ray.direction.getNormalized().y + 1.f);
    const vec3 sky_color = lerp(vec3(1, 1, 1), vec3(0.5f, 0.7f, 1.f), t);

    return sky_color * attenuation;
}