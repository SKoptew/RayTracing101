#ifndef __CAMERA_H_INCLUDED
#define __CAMERA_H_INCLUDED

#include "Ray.h"

class Camera
{
public:
    Camera(real aspect) { m_aspect = aspect; }

    void Set(const vec3 &origin, const vec3 &direction, real fovVertDegree, real focus_distance, real aperture);
    Ray GetRay(real u, real v) const;

    real ClipNear() const { return m_clipNear; }
    real ClipFar()  const { return m_clipFar; }

private:
    real m_clipNear   = real(0.01);
    real m_clipFar    = real(FLT_MAX);
    real m_aspect     = 1;
    real m_fov        = 60.0; // vertical
    real m_focus_dist = m_clipNear;
    real m_aperture05 = 0;
    vec3 m_origin     = { 0,0,0 };    

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
    if (m_aperture05 > 0)
    {
        real du, dv;
        RandUnitVectorDisk(du, dv);

        const vec3 scatter = m_horiz * (du*m_aperture05) + m_vert * (dv*m_aperture05);

        return Ray(m_origin + scatter, (m_UV_bottomLeft + u * m_UV_horiz + v * m_UV_vert - scatter).getNormalized());
    }

    return Ray(m_origin, (m_UV_bottomLeft + u * m_UV_horiz + v * m_UV_vert).getNormalized());
}

void Camera::Set(const vec3 &origin, const vec3 &direction, real fovVertDegree, real focus_distance, real aperture)
{
    m_fov        = fovVertDegree;
    m_focus_dist = focus_distance;
    m_aperture05 = aperture * real(0.5);
    m_origin     = origin;
    m_direction  = direction.getNormalized();
    
    m_horiz = cross(m_direction, { 0,1,0 });
    m_vert  = cross(m_horiz, m_direction);
    

    const real focusplane_height = 2*focus_distance * tan(ToRadians(fovVertDegree) * real(0.5));
    const real focusplane_width  = focusplane_height * m_aspect;
    
    m_UV_horiz = m_horiz * focusplane_width;
    m_UV_vert  = m_vert  * focusplane_height;

    m_UV_bottomLeft = m_direction*focus_distance - real(0.5)*m_UV_horiz - real(0.5)*m_UV_vert;
}

#endif