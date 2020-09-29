#ifndef __CAMERA_H_INCLUDED
#define __CAMERA_H_INCLUDED

#include "Ray.h"

class Camera
{
public:
    Camera(real aspect) { m_aspect = aspect; }

    void Set(const vec3 &origin, const vec3 &direction, real fovVertDegree);
    Ray GetRay(real u, real v) const;

    real ClipNear() const { return m_clipNear; }
    real ClipFar()  const { return m_clipFar; }

private:
    real m_clipNear  = real(0.01);
    real m_clipFar   = real(FLT_MAX);
    real m_aspect    = 1;
    real m_fov       = 60.0; // vertical
    vec3 m_origin    = { 0,0,0 };    

    //-- camera basis
    vec3 m_direction = { 0,0,-1 };
    vec3 m_horiz     = { 1,0,0 };
    vec3 m_vert      = { 0,1,0 };

    //-- UV vector basis
    vec3 m_UV_bottomLeft;
    vec3 m_UV_horiz;
    vec3 m_UV_vert;
};

Ray Camera::GetRay(real u, real v) const
{
    return Ray(m_origin, (m_UV_bottomLeft + u * m_UV_horiz + v * m_UV_vert).getNormalized());
}

void Camera::Set(const vec3 &origin, const vec3 &direction, real fovVertDegree)
{
    m_fov       = fovVertDegree;
    m_origin    = origin;
    m_direction = direction.getNormalized();
    
    m_horiz = cross(m_direction, { 0,1,0 });
    m_vert  = cross(m_horiz, m_direction);
    

    const real height = 2 * tan(ToRadians(fovVertDegree) * real(0.5)) * m_clipNear;
    const real width  = height * m_aspect;
    
    m_UV_horiz = m_horiz * width;
    m_UV_vert  = m_vert  * height;

    m_UV_bottomLeft = m_direction * m_clipNear - real(0.5)*m_UV_horiz - real(0.5)*m_UV_vert;
}

#endif