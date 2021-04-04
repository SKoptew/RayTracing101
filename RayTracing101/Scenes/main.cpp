#include "Utils/Image.h"
#include "Rendering/Rendering.h"
#include "Rendering/Material.h"

#include <ctime>
#include <iostream>
#include <iomanip>

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

void Render()
{
    auto image = new Image(1920/4, 1200/4);
    image->Clear();

    //-- camera && scene
    Camera camera(real(image->Width()) / image->Height());
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

    Rendering renderer;
    renderer.Render(scene, camera, *image);

    image->SaveToBMP("out.bmp");
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
    auto emissive0 = std::make_shared<Emissive>(Color(255,255,255), 4);

    auto lamb0 = std::make_shared<Lambertian>(Color(60, 100, 180));
    auto lamb1 = std::make_shared<Lambertian>(Color(60, 100, 80));

    auto metal0 = std::make_shared<Metal>(Color(200, 200, 200), (real)0.0);
    auto metal1 = std::make_shared<Metal>(Color(200, 50, 50), (real)0.3);
    auto metal2 = std::make_shared<Metal>(Color(50, 50, 200), (real)0.0);

    auto refr0 = std::make_shared<Refractive>((real)1.5);
    auto refr1 = std::make_shared<Refractive>((real)1.7);

    //-- scene
    scene.Add(new Sphere(vec3( 0, -100.5, -1), 100, std::make_shared<Lambertian>(Color(200, 200, 30))));
    scene.Add(new Sphere(vec3( 0, 0, -1), 0.5, emissive0)); // center ball
    scene.Add(new Sphere(vec3(-1, 0, -1), 0.5, lamb0));     // left   ball
    scene.Add(new Sphere(vec3( 1, 0, -1), 0.5, lamb1));     // right  ball
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