#ifndef __CAMERA_H_INCLUDED
#define __CAMERA_H_INCLUDED

#include "Rendering/Ray.h"

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

#endif