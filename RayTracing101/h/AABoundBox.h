#ifndef __AA_BOUND_BOX_H_INCLUDED
#define __AA_BOUND_BOX_H_INCLUDED

#include <algorithm>
#include "Math101.h"
#include "Ray.h"

class AABoundBox
{
public:
    AABoundBox() : pmin(vec3(-FLT_MAX)), pmax(vec3(FLT_MAX)) {}
    AABoundBox(const vec3 &_pmin, const vec3 &_pmax) : pmin(_pmin), pmax(_pmax) {}

    vec3 GetMin() const { return pmin; }
    vec3 GetMax() const { return pmax; }
    vec3 Size  () const { return pmax - pmin; }

    bool Hit(const Ray &ray, real t_min, real t_max) const
    {
        //-- intersect with YZ planes
        real t0 = (pmin.x - ray.origin.x) * ray.inv_direction.x;
        real t1 = (pmax.x - ray.origin.x) * ray.inv_direction.x;

        if (ray.inv_direction.x < 0)
            std::swap(t0, t1);

        t_min = Max(t_min, t0);
        t_max = Min(t_max, t1);

        if (t_min >= t_max)
            return false;

        //-- intersect with XZ planes
        t0 = (pmin.y - ray.origin.y) * ray.inv_direction.y;
        t1 = (pmax.y - ray.origin.y) * ray.inv_direction.y;

        if (ray.inv_direction.y < 0)
            std::swap(t0, t1);

        t_min = Max(t_min, t0);
        t_max = Min(t_max, t1);

        if (t_min >= t_max)
            return false;


        //-- intersect with XY planes
        t0 = (pmin.z - ray.origin.z) * ray.inv_direction.z;
        t1 = (pmax.z - ray.origin.z) * ray.inv_direction.z;

        if (ray.inv_direction.z < 0)
            std::swap(t0, t1);

        t_min = Max(t_min, t0);
        t_max = Min(t_max, t1);

        if (t_min >= t_max)
            return false;

        return true;
    }

    void Expand(const AABoundBox &rhs)
    {
        pmin = Min(pmin, rhs.pmin);
        pmax = Max(pmax, rhs.pmax);
    }

private:
    vec3 pmin;
    vec3 pmax;
};

#endif