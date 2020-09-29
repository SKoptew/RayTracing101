#include "h/Image.h"
#include "h/Scene.h"
#include "h/Camera.h"
#include "h/Random101.h"

#include <ctime>
#include <iostream>
#include <iomanip>

const int SAMPLES_CNT = 64;
const real SAMPLES_CNT_INV = 1 / real(SAMPLES_CNT);

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

void Render()
{
    auto img = new Image(512, 256);
    img->Clear();

    int w = img->Width();
    int h = img->Height();
    auto data = img->GetData();

    const real inv_w = 1 / real(w);
    const real inv_h = 1 / real(h);

    //-- camera
    Camera camera(real(w) / h);
    camera.Set(vec3(0, 0, 0), vec3(0, 0, -1), 90);

    //-- scene
    Scene scene;
    scene.Add(new Sphere(vec3(0, 0, -1), 0.5));
    scene.Add(new Sphere(vec3(0, -100.5, -1), 100));
        
    const real clip_far = camera.ClipFar();

    for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x)
    {
        vec3 color = { 0,0,0 };
        for (int sample_num = 0; sample_num < SAMPLES_CNT; ++sample_num)
        {
            const real u = (x + (Rand01()*real(0.5)-1)) * inv_w;
            const real v = (y + (Rand01()*real(0.5)-1)) * inv_h;

            const auto ray = camera.GetRay(u, v);

            HitRecord hit;        
            if (scene.Hit(ray, 0, clip_far, hit))
            {
                color += hit.nrm * 0.5 + vec3(0.5);
            }
            else
            {
                const float t = 0.5f * (ray.direction.getNormalized().y() + 1.f);
                color += lerp(vec3(1,1,1), vec3(0.5f, 0.7f, 1.f), t);
            }
        }
        color *= SAMPLES_CNT_INV;

        img->SetPixel(x, y, color);
    }

    img->SaveToBMP("out.bmp");
}