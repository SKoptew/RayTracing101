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

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

static std::mt19937 gen{ std::random_device() () };

real Rand01()
{   
    //static std::uniform_real_distribution<real> distr{ 0, 1 };
    //return distr(gen);

    return std::generate_canonical<real, std::numeric_limits<real>::digits>(gen);
}

vec3 RandUnitVector()
{
    //-- rejection sampling
    vec3 pt;
    real length2;
    do
    {
        pt.x = Rand01() * 2 - 1;
        pt.y = Rand01() * 2 - 1;
        pt.z = Rand01() * 2 - 1;

        length2 = pt.length2();
    } 
    while (length2 > 1);

    return pt / sqrt(length2);
}
