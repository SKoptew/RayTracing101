#ifndef __RAY_H_INCLUDED
#define __RAY_H_INCLUDED

#include "h/Math101.h"

struct Ray
{
    vec3 origin;
    vec3 direction; // unit length assumed in intersection tests

    Ray() {}
    Ray(const vec3 &_origin, const vec3 &_direction) 
        : origin   (_origin), 
          direction(_direction) 
    {}

    vec3 GetPointAt(real t) const
    {
        return origin + direction * t;
    }
};

#endif