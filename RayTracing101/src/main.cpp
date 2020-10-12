#include "h/Image.h"
#include "h/Scene.h"
#include "h/Camera.h"
#include "h/Material.h"

#include <ctime>
#include <iostream>
#include <iomanip>
#include <memory>

//#define DBG_RENDER_BOUNCES_COUNT
#define DBG_SHOW_BOUNCES_OVERRUN

const int  SAMPLES_CNT  = 32;
const real MIN_HIT_DIST = (real)0.0001;
const int  MAX_BOUNCES  = 16;

void Render();

int main(int argc, char* argv[])
{
    auto c_start = std::clock();
    Render();
    auto c_end = std::clock();

    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
        << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n";

    return 0;
}

vec3 CalcRayColor(const Ray &_ray, const Scene &scene, real clip_far)
{
    vec3 acc_attenuation = vec3(1);

    Ray ray_in = _ray;
    Ray ray_out;
    HitRecord hit;
    vec3 attenuation;

    int bounce = 0;    
    for (; bounce < MAX_BOUNCES; ++bounce)
    {
        if (scene.Hit(ray_in, MIN_HIT_DIST, bounce == 0 ? clip_far : real(FLT_MAX), hit))
        {            
            if (hit.mat->Scatter(ray_in, hit, attenuation, ray_out))
            {
                acc_attenuation *= attenuation;
                ray_in = ray_out;
            }
            else
                return vec3(0); // ray absorbed
        }
        else
            break;
    }

#ifdef DBG_SHOW_BOUNCES_OVERRUN
    if (bounce == MAX_BOUNCES)
        return vec3(1, 0, 1);
#endif

#ifdef DBG_RENDER_BOUNCES_COUNT
    return vec3(real(bounce) / MAX_BOUNCES);
#endif

    //-- imitation of sky color
    const real t = 0.5f * (ray_in.direction.getNormalized().y + 1.f);
    const vec3 sky_color = lerp(vec3(1, 1, 1), vec3(0.5f, 0.7f, 1.f), t);

    return acc_attenuation * sky_color;
}

void Render()
{
    auto img = new Image(1024, 576);
    img->Clear();

    int w = img->Width();
    int h = img->Height();
    auto data = img->GetData();

    const real inv_w = 1 / real(w);
    const real inv_h = 1 / real(h);

    //-- camera
    Camera camera(real(w) / h);
    camera.Set(vec3(0, 0, 0), vec3(0, 0, -1), 90);

    //-- materials
    auto mat_ground = std::make_shared<Lambertian>(Color(200, 200, 30));

    auto mat1 = std::make_shared<Lambertian>(Color(204,204,0));
    auto mat2 = std::make_shared<Lambertian>(Color(200, 200, 200));

    auto metal0 = std::make_shared<Metal>(Color(200, 200, 200), (real)0.0);
    auto metal1 = std::make_shared<Metal>(Color(200, 50,   50), (real)0.3);
    auto metal2 = std::make_shared<Metal>(Color(50,  50,  200), (real)0.0);

    auto refr0 = std::make_shared<Refractive>((real)1.5);
    auto refr1 = std::make_shared<Refractive>((real)1.7);

    //-- scene
    Scene scene;
    scene.Add(new Sphere(vec3(0, -100.5, -1), 100, mat_ground.get()));

    scene.Add(new Sphere(vec3( 0, 0.0, -1),  0.5, refr0.get())); // center
    scene.Add(new Sphere(vec3(-1,   0, -1),  0.5, refr1.get())); // left
    scene.Add(new Sphere(vec3( 1,   0, -1), -0.5, refr0.get())); // right
        

    const real clip_far = camera.ClipFar();

    for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x)
    {
        int samples_ok = 0;
        vec3 color = { 0,0,0 };
        for (int sample_num = 0; sample_num < SAMPLES_CNT; ++sample_num)
        {
            const real u = (x + (Rand01()*real(0.5)-1)) * inv_w;
            const real v = (y + (Rand01()*real(0.5)-1)) * inv_h;

            const vec3 sample_color = CalcRayColor(camera.GetRay(u, v), scene, clip_far);

            if (!sample_color.isNaN())
            {
                color += sample_color;
                ++samples_ok;
            }
        }
        color /= real(samples_ok);
        
#ifndef DBG_RENDER_BOUNCES_COUNT
        color = LinearToSrgb(color);
#endif
        img->SetPixel(x, y, color);
    }

    img->SaveToBMP("out.bmp");
}