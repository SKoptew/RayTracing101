#ifndef __MATH_101_H_INCLUDED
#define __MATH_101_H_INCLUDED

#define _USE_MATH_DEFINES
#include <cmath>

template<typename T>
class vec3_t
{
public:
    T x;
    T y;
    T z;

    vec3_t() {}
    vec3_t(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }
    vec3_t(T xyz) { x = y = z = xyz; }

    //-- unary
    vec3_t operator - () const { return vec3_t(-x, -y, -z); }

    //-- binary, modify itself
    vec3_t& operator += (const vec3_t &rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
    vec3_t& operator -= (const vec3_t &rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
    vec3_t& operator *= (const vec3_t &rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
    vec3_t& operator /= (const vec3_t &rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

    vec3_t& operator *= (T a) { x *= a; y *= a; z *= a; return *this; }
    vec3_t& operator /= (T a) { x /= a; y /= a; z /= a; return *this; }

    //-- binary, const
    vec3_t operator + (const vec3_t &rhs) const { return vec3_t(x + rhs.x, y + rhs.y, z + rhs.z); }
    vec3_t operator - (const vec3_t &rhs) const { return vec3_t(x - rhs.x, y - rhs.y, z - rhs.z); }
    vec3_t operator * (const vec3_t &rhs) const { return vec3_t(x * rhs.x, y * rhs.y, z * rhs.z); }
    vec3_t operator / (const vec3_t &rhs) const { return vec3_t(x / rhs.x, y / rhs.y, z / rhs.z); }

    vec3_t operator * (T a) const { return vec3_t(x * a, y * a, z * a); }
    vec3_t operator / (T a) const { return vec3_t(x / a, y / a, z / a); }

    T length () const { return sqrt(x*x + y*y + z*z); }
    T length2() const { return      x*x + y*y + z*z;  }

    vec3_t& normalize()
    {
        const T k = 1 / sqrt(x*x + y*y + z*z);
        x *= k, y *= k, z *= k;
        return *this;
    }

    vec3_t getNormalized() const
    {
        const T k = 1 / sqrt(x*x + y*y + z*z);
        return vec3_t(x*k, y*k, z*k);
    }
};

template<typename T>
vec3_t<T> operator * (T a, const vec3_t<T> &vec)
{
    return vec3_t<T>(a*vec.x, a*vec.y, a*vec.z);
}

template<typename T>
T dot(const vec3_t<T> &v0, const vec3_t<T> &v1)
{
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
}

template<typename T>
vec3_t<T> cross(const vec3_t<T> &v0, const vec3_t<T> &v1)
{
    return vec3_t<T>((v0.y*v1.z - v0.z*v1.y),
                   (-(v0.x*v1.z - v0.z*v1.x)),
                     (v0.x*v1.y - v0.y*v1.x));
}

template<typename T>
vec3_t<T> lerp(const vec3_t<T> &v0, const vec3_t<T> &v1, T t)
{
    return (1 - t)*v0 + t * v1;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

using real = float;
using vec3 = vec3_t<real>;

#define ToRadians(angleDeg) (angleDeg * real(M_PI/180))
#define ToDegrees(angleRad) (angleRad * real(180/M_PI))

vec3 LinearToSrgb(const vec3 &c);
vec3 SrgbToLinear(const vec3 &c);

real Rand01();
vec3 RandUnitVector();

#endif