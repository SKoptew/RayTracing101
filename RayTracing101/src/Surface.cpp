#include "h/Hitable.h"

bool Sphere::Hit(const Ray &ray, real t_min, real t_max, HitRecord &hit) const
{
    const vec3 oc = ray.origin - m_center;
    //const real A = dot(ray.direction, ray.direction); // == 1, because of unit length of ray.direction
    //const real B = 2 * dot(ray.direction, oc);
    const real half_B = dot(ray.direction, oc);
    const real C = dot(oc, oc) - m_radius2;

    const real discr = half_B*half_B - C; // 1/4*discriminant

    if (discr > 0)
    {
        const real root = sqrt(discr);

        real t = -half_B - root;

        if (t > t_min && t < t_max)
        {
            hit.t = t;
            hit.pt = ray.GetPointAt(t);
            hit.nrm = (hit.pt - m_center) * m_radiusInv;
            hit.mat = m_material.get();
            return true;
        }

        //-- inner surface of sphere
        t = -half_B + root;

        if (t > t_min && t < t_max)
        {
            hit.t = t;
            hit.pt = ray.GetPointAt(t);
            hit.nrm = (hit.pt - m_center) * m_radiusInv;
            hit.mat = m_material.get();
            return true;
        }
    }

    return false;
}

AABoundBox Sphere::GetAABoundBox() const
{
    return AABoundBox(m_center - vec3(m_radius, m_radius, m_radius),
                      m_center + vec3(m_radius, m_radius, m_radius));
}
