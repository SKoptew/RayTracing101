#include "h/Image.h"
#include "h/Scene.h"
#include "h/Camera.h"

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

    //-- camera
    Camera camera(real(w) / h);
    camera.Set(vec3(0, 0, 0), vec3(0, 0, -1), 90);

    //-- scene
    Scene scene;
    scene.Add(new Sphere(vec3(0, 0, -1), 0.5));
    scene.Add(new Sphere(vec3(0, -100.5, -1), 100));

    vec3 color(1, 0, 0);
    const real clip_far = camera.ClipFar();

    for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x)
    {
        const real u = real(x) / w;
        const real v = real(y) / h;

        const auto ray = camera.GetRay(u, v);

        HitRecord hit;        
        if (scene.Hit(ray, 0, clip_far, hit))
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