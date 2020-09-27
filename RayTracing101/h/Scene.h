#ifndef __SCENE_H_INCLUDED
#define __SCENE_H_INCLUDED

#include <vector>
#include "h/Hitable.h"

class Scene : public Hitable
{
public:
    Scene() {}
    ~Scene()
    {
        Clear();
    }

    void Add(Hitable *surface)
    {
        m_surfaces.push_back(surface);
    }

    void Clear()
    {
        for (auto surf : m_surfaces)
            delete surf;
        m_surfaces.clear();
    }

    bool Hit(const Ray &ray, real t_min, real t_max, HitRecord &rec) const override
    {
        HitRecord hit;
        bool hitAnything = false;
        real t_closest   = t_max;

        for (auto surface : m_surfaces)
        {
            if (surface->Hit(ray, t_min, t_closest, hit))
            {
                hitAnything = true;
                t_closest = hit.t;
                rec = hit;
            }
        }

        return hitAnything;
    }

private:
    std::vector<Hitable*> m_surfaces;
};

#endif