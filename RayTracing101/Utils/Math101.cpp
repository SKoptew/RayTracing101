#include "Utils/Math101.h"
#include "Utils/Random.h"

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

real Rand01()
{
    return GetRandom01<real>();
}

real Rand(real min, real max)
{
    return min + (max - min)*Rand01();
}

vec3 RandUnitVectorSphere()
{
    /*
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

    return pt.getNormalized();*/

    while (true) 
    {
        const vec3 p = vec3(Rand(-1, 1), Rand(-1, 1), Rand(-1, 1));
        const real length2 = p.length2();

        if (length2 < 1)
            return p / sqrt(length2);
    }
}

vec3 RandUnitVectorSemisphere(const vec3 &normal)
{
    vec3 vec = RandUnitVectorSphere();
    return dot(vec, normal) > 0 ? vec : -vec;
}

void RandUnitVectorDisk(real &u, real&v)
{
    const real r = sqrt(Rand01());
    const real a = Rand01() * (M_PI*2);
    
    u = r * cos(a);
    v = r * sin(a);
}

vec3 RandColor()
{
    return vec3(Rand01(), Rand01(), Rand01());
}

vec3 RandColor(real min, real max)
{
    return vec3(min + (max - min)*Rand01(), min + (max - min)*Rand01(), min + (max - min)*Rand01());
}