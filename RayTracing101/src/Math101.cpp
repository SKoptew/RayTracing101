#include "h/Math101.h"
#include <random>

const real GAMMA     = real(2.2);
const real GAMMA_INV = 1 / GAMMA;

vec3 LinearToSrgb(const vec3 &c)
{
    return vec3(pow(c.x, GAMMA_INV),
                pow(c.y, GAMMA_INV),
                pow(c.z, GAMMA_INV));
}

vec3 SrgbToLinear(const vec3 &c)
{
    return vec3(pow(c.x, GAMMA),
                pow(c.y, GAMMA),
                pow(c.z, GAMMA));
}

vec3 Color(int r, int g, int b)
{
    const auto k = 1 / real(255);

    return vec3(pow(r * k, GAMMA),
                pow(g * k, GAMMA),
                pow(b * k, GAMMA));
}

vec3 Reflect(const vec3 &v, const vec3 &n)
{
    return v - n * 2*dot(v, n);
}

vec3 Reflect(const vec3 &v, const vec3 &n, real VdotN)
{
    return v - n * 2*VdotN;
}

//-- normal directed from medium2 towards medium1
vec3 Refract(const vec3 &v, const vec3 &n, real VdotN, real n1_n2)
{

    const vec3 vt_tg = n1_n2 * (v - n * VdotN);
    const real discr = 1 - vt_tg.length2();

    if (discr > 0) // refract
    {
        const vec3 vt_n = n * Sign(VdotN) * sqrt(discr);
        return vt_tg + vt_n;
    }

    return v - n * 2*VdotN; // total internal reflection
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

vec3 RandUnitVectorInSemisphere(const vec3 &normal)
{
    vec3 vec = RandUnitVector();
    return dot(vec, normal) > 0 ? vec : -vec;
}

