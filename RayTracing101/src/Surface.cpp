#include "h/Hitable.h"

bool Sphere::Hit(const Ray &ray, real t_min, real t_max, HitRecord &hit) const
{
    const vec3 oc = ray.origin - m_center;
    //const real A = dot(ray.direction, ray.direction); // == 1, because of unit length of ray.direction
    const real B = 2 * dot(ray.direction, oc);
    const real C = dot(oc, oc) - m_radius2;

    const real discr = B*B - 4*C;

    if (discr > 0)
    {
        real t = (-B - sqrt(discr)) * real(0.5);

        if (t > t_min && t < t_max)
        {
            hit.t = t;
            hit.pt = ray.GetPointAt(t);
            hit.nrm = (hit.pt - m_center) * m_radiusInv;
            return true;
        }

        //-- inner surface of sphere
        t = (-B + sqrt(discr)) * real(0.5);

        if (t > t_min && t < t_max)
        {
            hit.t = t;
            hit.pt = ray.GetPointAt(t);
            hit.nrm = (hit.pt - m_center) * m_radiusInv;
            return true;
        }
    }

    return false;
}
