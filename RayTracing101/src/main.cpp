#include "h/Image.h"
#include "h/Scene.h"
#include "h/Camera.h"
#include "h/Material.h"

#include <ctime>
#include <iostream>
#include <iomanip>
#include <memory>

const int  SAMPLES_CNT = 64;
const real SAMPLES_CNT_INV = 1 / real(SAMPLES_CNT);

const real MIN_HIT_DIST = (real)0.001;
const int  MAX_DEPTH = 15;

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

vec3 CalcRayColor(const Ray &ray, const Scene &scene, real clip_far, int depth)
{
    HitRecord hit;

    if (scene.Hit(ray, MIN_HIT_DIST, clip_far, hit))
    {
        Ray ray_scattered;
        vec3 attenuation;

        if (depth < MAX_DEPTH && hit.mat->Scatter(ray, hit, attenuation, ray_scattered))
        {
            return attenuation * CalcRayColor(ray_scattered, scene, real(FLT_MAX), depth - 1); // reflected ray can hit surface at infinite distance, but first (camera->surface) - only at less to camera.clip_far
        }
        return vec3(0);
    }
    else
    {
        //-- imitation of sky color
        const float t = 0.5f * (ray.direction.getNormalized().y + 1.f);
        return lerp(vec3(1, 1, 1), vec3(0.5f, 0.7f, 1.f), t);
    }
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
    auto mat1 = std::make_shared<Lambertian>(vec3(0.8, 0.3, 0.3));
    auto mat2 = std::make_shared<Lambertian>(vec3(0.8, 0.8, 0.0));
    auto mat3 = std::make_shared<Metal>     (vec3(0.8, 0.6, 0.2), 0.f);
    auto mat4 = std::make_shared<Metal>     (vec3(0.8, 0.8, 0.8), 1.f);

    //-- scene
    Scene scene;
    scene.Add(new Sphere(vec3( 0,      0,  -1), 0.5, mat1.get())); // diffuse ball
    scene.Add(new Sphere(vec3( 0, -100.5,  -1), 100, mat2.get())); // diffuse large bottom surface

    scene.Add(new Sphere(vec3( 1, 0, -1), 0.5, mat3.get())); // two metal balls
    scene.Add(new Sphere(vec3(-1, 0, -1), 0.5, mat4.get()));
        

    const real clip_far = camera.ClipFar();

    for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x)
    {
        vec3 color = { 0,0,0 };
        for (int sample_num = 0; sample_num < SAMPLES_CNT; ++sample_num)
        {
            const real u = (x + (Rand01()*real(0.5)-1)) * inv_w;
            const real v = (y + (Rand01()*real(0.5)-1)) * inv_h;

            color += CalcRayColor(camera.GetRay(u, v), scene, clip_far, 0);
        }
        color *= SAMPLES_CNT_INV;

        img->SetPixel(x, y, LinearToSrgb(color));
    }

    img->SaveToBMP("out.bmp");
}