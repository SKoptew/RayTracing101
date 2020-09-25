#include "h/Image.h"
#include "h/Math101.h"
#include "h/Ray.h"

void Render();

int main(int argc, char* argv[])
{
    Render();   

    return 0;
}

bool HitSphere(const vec3 &center, real rad, const Ray &ray)
{
    const vec3 oc = ray.origin - center;
    const real A = dot(ray.direction, ray.direction);
    const real B = 2*dot(ray.direction, oc);    
    const real C = dot(oc, oc) - rad*rad;

    const real discr = B*B - 4*A*C;

    return discr > 0;
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

    for (int y = 0; y < h; ++y)
    for (int x = 0; x < w; ++x)
    {
        const real u = real(x) / w;
        const real v = real(y) / h;

        const Ray ray(origin, bl_corner + u*horiz + v*vert);

        vec3 col(1,0,0);

        if (!HitSphere(vec3(0,0,-1), 0.5f, ray))
        {
            const float t = 0.5f * (ray.direction.getNormalized().y() + 1.f);
            col = lerp(vec3(1,1,1), vec3(0.5f, 0.7f, 1.f), t);            
        }

        img->SetPixel(x, y, col);
    }

    img->SaveToBMP("out.bmp");
}