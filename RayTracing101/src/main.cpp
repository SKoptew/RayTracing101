#include "h/Image.h"
#include "h/Scene.h"
#include <float.h>

void Render();

int main(int argc, char* argv[])
{
    Render();   

    return 0;
}

void Render()
{
    auto img = new Image(512, 256);
    img->Clear();

    int w = img->Width();
    int h = img->Height();
    auto data = img->GetData();

    //-- tiny camera model
    const vec3 origin(0, 0, 0);
    const vec3 bl_corner(-2, -1, -1);
    const vec3 horiz(4, 0, 0);
    const vec3 vert (0, 2, 0);

    //-- scene
    Scene scene;
    scene.Add(new Sphere(vec3(0, 0, -1), 0.5));
    scene.Add(new Sphere(vec3(0, -100.5, -1), 100));

    vec3 color(1, 0, 0);

    for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x)
    {
        const real u = real(x) / w;
        const real v = real(y) / h;

        const Ray ray(origin, bl_corner + u*horiz + v*vert);

        HitRecord hit;        
        if (scene.Hit(ray, 0, FLT_MAX, hit))
        {
            color = hit.nrm * 0.5 + vec3(0.5);
        }
        else
        {
            const float t = 0.5f * (ray.direction.getNormalized().y() + 1.f);
            color = lerp(vec3(1,1,1), vec3(0.5f, 0.7f, 1.f), t);
        }

        img->SetPixel(x, y, color);
    }

    img->SaveToBMP("out.bmp");
}