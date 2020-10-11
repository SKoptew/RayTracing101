#include "h/Math101.h"
#include <random>

const real GAMMA = real(2.2);

vec3 LinearToSrgb(const vec3 &c)
{
    return vec3(pow(c.x, 1 / GAMMA),
                pow(c.y, 1 / GAMMA),
                pow(c.z, 1 / GAMMA));
}

vec3 SrgbToLinear(const vec3 &c)
{
    return vec3(pow(c.x, GAMMA),
                pow(c.y, GAMMA),
                pow(c.z, GAMMA));
}

vec3 Reflect(const vec3 &v, const vec3 &n)
{
    return v - 2 * dot(v,n) * n;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

static std::mt19937 gen{ std::random_device() () };

real Rand01()
{
    return std::generate_canonical<real, std::numeric_limits<real>::digits>(gen);
}

vec3 RandUnitVector()
{
    //-- Marsaglia, George. Choosing a Point from the Surface of a Sphere. Ann. Math. Statist. 43 (1972), no. 2, 645--646
    vec3 pt;
    do
    {
        pt.x = Rand01() * 2 - 1;      // v1
        pt.y = Rand01() * 2 - 1;      // v2
        pt.z = pt.x*pt.x + pt.y*pt.y; // S
    }
    while (pt.z >= 1);

    real m = 2 * sqrt(pt.z);

    pt.x *= m;
    pt.y *= m;
    pt.z = 1 - 2*pt.z;

    return pt;
}

