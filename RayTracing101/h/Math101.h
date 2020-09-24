#ifndef __MATH_101_H_INCLUDED
#define __MATH_101_H_INCLUDED

#include <cmath>

template<typename T>
class vec3_t
{
public:
    T d[3];

    vec3_t() {}
    vec3_t(T x, T y, T z) { d[0] = x; d[1] = y; d[2] = z; }

    const T x() const { return d[0]; }
    const T y() const { return d[1]; }
    const T z() const { return d[2]; }
    const T r() const { return d[0]; }
    const T g() const { return d[1]; }
    const T b() const { return d[2]; }

    //-- access
    T operator [] (int i) const { return d[i]; }
    T operator [] (int i)       { return d[i]; }

    //-- unary
    vec3_t operator - () const { return vec3_t(-d[0], -d[1], -d[2]); }

    //-- binary
    vec3_t& operator += (const vec3_t &rhs) { d[0] += rhs.d[0]; d[1] += rhs.d[1]; d[2] += rhs.d[2]; return *this; }
    vec3_t& operator -= (const vec3_t &rhs) { d[0] -= rhs.d[0]; d[1] -= rhs.d[1]; d[2] -= rhs.d[2]; return *this; }
    vec3_t& operator *= (const vec3_t &rhs) { d[0] *= rhs.d[0]; d[1] *= rhs.d[1]; d[2] *= rhs.d[2]; return *this; }
    vec3_t& operator /= (const vec3_t &rhs) { d[0] /= rhs.d[0]; d[1] /= rhs.d[1]; d[2] /= rhs.d[2]; return *this; }

    vec3_t& operator *= (T a) { d[0] *= a; d[1] *= a; d[2] *= a; return *this; }
    vec3_t& operator /= (T a) { d[0] /= a; d[1] /= a; d[2] /= a; return *this; }

    vec3_t operator + (const vec3_t &rhs) { return vec3_t(d[0] + rhs.d[0], d[1] + rhs.d[1], d[2] + rhs.d[2]); }
    vec3_t operator - (const vec3_t &rhs) { return vec3_t(d[0] - rhs.d[0], d[1] - rhs.d[1], d[2] - rhs.d[2]); }
    vec3_t operator * (const vec3_t &rhs) { return vec3_t(d[0] * rhs.d[0], d[1] * rhs.d[1], d[2] * rhs.d[2]); }
    vec3_t operator / (const vec3_t &rhs) { return vec3_t(d[0] / rhs.d[0], d[1] / rhs.d[1], d[2] / rhs.d[2]); }

    vec3_t operator * (T a) const { return vec3_t(d[0] * a, d[1] * a, d[2] * a); }
    vec3_t operator / (T a) const { return vec3_t(d[0] / a, d[1] / a, d[2] / a); }

    T length () const { return sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]); }
    T length2() const { return      d[0]*d[0] + d[1]*d[1] + d[2]*d[2];  }

    vec3_t& normalize()
    {
        const T k = 1 / sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);
        d[0] *= k, d[1] *= k, d[2] *= k;
        return *this;
    }

    vec3_t getNormalized()
    {
        const T k = 1 / sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);
        return vec3_t(d[0]*k, d[1]*k, d[2]*k);
    }
};

template<typename T>
vec3_t<T> operator * (T a, const vec3_t<T> &vec)
{
    return vec3_t<T>(a*vec.d[0], a*vec.d[1], a*vec.d[2]);
}

template<typename T>
T dot(const vec3_t<T> &v0, const vec3_t<T> &v1)
{
    return v0.d[0]*v1.d[0] + v0.d[1]*v1.d[1] + v0.d[2]*v1.d[2];
}

template<typename T>
vec3_t<T> cross(const vec3_t<T> &v0, const vec3_t<T> &v1)
{
    return vec3_t<T>((v0.d[1]*v1.d[2] - v0.d[2]*v1.d[1]),
                   (-(v0.d[0]*v1.d[2] - v0.d[2]*v1.d[0])),
                     (v0.d[0]*v1.d[1] - v0.d[1]*v1.d[0]));
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

using real = float;
using vec3 = vec3_t<real>;

inline int Color(const vec3 &c)
{
    const int _r = int(c.r()*255.99f) & 0xff;
    const int _g = int(c.g()*255.99f) & 0xff;
    const int _b = int(c.b()*255.99f) & 0xff;

    return (0xff << 24) | (_r << 16) | (_g << 8) | _b;
}

#endif