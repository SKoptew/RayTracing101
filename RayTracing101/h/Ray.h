#ifndef __RAY_H_INCLUDED
#define __RAY_H_INCLUDED

#include "h/Math101.h"

struct Ray
{
    vec3 origin;
    vec3 direction; // unit length assumed in intersection tests
    vec3 inv_direction;

    Ray() = default;
    Ray(const vec3 &_origin, const vec3 &_direction) 
        : origin   (_origin), 
          direction(_direction),
          inv_direction(vec3(1/_direction.x, 1/_direction.y, 1/_direction.z))
    {
    }

    vec3 GetPointAt(real t) const
    {
        return origin + direction * t;
    }
};

#endif