#include "h/Image.h"
#include "h/Scene.h"
#include "h/Camera.h"
#include "h/Material.h"

#include <ctime>
#include <iostream>
#include <iomanip>
#include <memory>

//#define DBG_RENDER_BOUNCES_COUNT
//#define DBG_SHOW_BOUNCES_OVERRUN
//#define CHECK_NAN

const int  SAMPLES_CNT  = 256;
const real MIN_HIT_DIST = (real)0.0001;
const int  MAX_BOUNCES  = 32;

void Render();
void TestRandom();
void CreateStaticScene(Scene &scene);
void CreateRandomScene(Scene &scene);

int main(int argc, char* argv[])
{
    auto c_start = std::clock();
    Render();
    //TestRandom();
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
                ray_in.inv_direction = vec3(1 / ray_in.direction.x, 1 / ray_in.direction.y, 1 / ray_in.direction.z);
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
    auto img = new Image(1920, 1200);
    img->Clear();

    int w = img->Width();
    int h = img->Height();
    auto data = img->GetData();

    const real inv_w = 1 / real(w-1);
    const real inv_h = 1 / real(h-1);

    //-- camera && scene
    Camera camera(real(w) / h);    
    Scene scene;

    if (0)
    {
        CreateStaticScene(scene);
        camera.Set(vec3(0, 0, 0), vec3(0, 0, -1), 90, 1, 0);
    }
    else
    {
        CreateRandomScene(scene);
        const vec3 lookFrom = vec3(13, 2, 3);
        const vec3 lookAt = vec3(0, 0.5, 0);
        camera.Set(lookFrom, lookAt - lookFrom, 20, 10.0, 0.05);
    }
    scene.Finalize();


    const real clip_far = camera.ClipFar();

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            int samples_ok = 0;
            vec3 color = { 0,0,0 };
            for (int sample_num = 0; sample_num < SAMPLES_CNT; ++sample_num)
            {
                const real u = (x + (Rand01()*real(0.5)-1)) * inv_w;
                const real v = (y + (Rand01()*real(0.5)-1)) * inv_h;

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
            img->SetPixel(x, y, color);
        }

        std::cerr << "\rProcessed: " << y*100 / h << "%..." << std::flush;
    }

    std::cerr << "\nDone\n";

    img->SaveToBMP("out.bmp");
}

void TestRandom()
{
    auto img = new Image(1920, 1200);
    img->Clear();

    int w = img->Width();
    int h = img->Height();
    auto data = img->GetData();

    for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x)
    {
        //float v = Get1dNoise01(x, y);
        float v = Rand01();

        img->SetPixel(x, y, vec3(v, v, v));
    }

    std::cerr << "\nDone\n";

    img->SaveToBMP("out.bmp");
}

void CreateStaticScene(Scene &scene)
{
    //-- materials
    auto lamb0 = std::make_shared<Lambertian>(Color(60, 100, 180));

    auto metal0 = std::make_shared<Metal>(Color(200, 200, 200), (real)0.0);
    auto metal1 = std::make_shared<Metal>(Color(200, 50, 50), (real)0.3);
    auto metal2 = std::make_shared<Metal>(Color(50, 50, 200), (real)0.0);

    auto refr0 = std::make_shared<Refractive>((real)1.5);
    auto refr1 = std::make_shared<Refractive>((real)1.7);

    //-- scene
    scene.Add(new Sphere(vec3( 0, -100.5, -1), 100, std::make_shared<Lambertian>(Color(200, 200, 30))));
    scene.Add(new Sphere(vec3( 0, 0, -1), 0.5, lamb0));  // center ball
    scene.Add(new Sphere(vec3(-1, 0, -1), 0.5, refr0));  // left   ball
    scene.Add(new Sphere(vec3( 1, 0, -1), 0.5, metal0)); // right  ball
}

void CreateRandomScene(Scene &scene)
{
    scene.Add(new Sphere(vec3(0, -1000, 0), 1000, std::make_shared<Lambertian>(vec3(0.5, 0.5, 0.5))));

    for (int a = -9; a < 10; ++a)
    for (int b = -5; b < 5; ++b)
    {   
        vec3 center(a + 0.6*Rand01(), 0.2, b + 0.9*Rand01());
        
        if ((center - vec3(4, 0.2, 0)).length() > 0.9) // avoid big spheres
        {
            auto choose_mat = Rand01();
    
            if (choose_mat < 0.55) // diffuse
            {
                auto albedo = RandColor(0.2,1.0)*RandColor(0.2,1.0);
                scene.Add(new Sphere(center, 0.2, std::make_shared<Lambertian>(albedo)));
            }
            else
            if (choose_mat < 0.8) // metal
            {
                auto albedo = RandColor(0.5, 1);
                auto fuzz = Rand(0, 0.5);
                scene.Add(new Sphere(center, 0.2, std::make_shared<Metal>(albedo, fuzz)));
            }
            else // refractive
            {
                auto attenuation = Rand01() > 0.7 ? RandColor(0.6, 0.95) : vec3(0.95);
                scene.Add(new Sphere(center, 0.2, std::make_shared<Refractive>(1.5, attenuation)));
            }
        }
    }

    scene.Add(new Sphere(vec3( 0, 1, 0), 1.0, std::make_shared<Refractive>(1.5, vec3(0.95))));
    scene.Add(new Sphere(vec3(-4, 1, 0), 1.0, std::make_shared<Lambertian>(Color(40,69,123))));
    scene.Add(new Sphere(vec3( 4, 1, 0), 1.0, std::make_shared<Metal>(vec3(0.7, 0.6, 0.5), 0.0)));
}