#ifndef __SCENE_H_INCLUDED
#define __SCENE_H_INCLUDED

#include <vector>
#include "h/KDTree.h"

class Scene : public Hitable
{
public:
    Scene() = default;
    ~Scene()
    {
        Clear();
    }

    void Add(Hitable *surface)
    {
        m_surfaces_to_add.push_back(surface);
    }

    void Finalize()
    {
        m_kdtree.reset();

        if (m_surfaces_to_add.size() > 0)
        {
            m_kdtree = std::make_unique<KDTree>(m_surfaces_to_add, 0, m_surfaces_to_add.size());
            m_surfaces_to_add.clear();
        }
    }

    void Clear()
    {
        m_kdtree.reset();
        m_surfaces_to_add.clear();
    }

    bool Hit(const Ray &ray, real t_min, real t_max, HitRecord &rec) const override
    {
        if (m_kdtree)
        {
            return m_kdtree->Hit(ray, t_min, t_max, rec);
        }

        return false;
    }

    AABoundBox GetAABoundBox() const override
    {
        if (m_kdtree)
        {
            return m_kdtree->GetAABoundBox();
        }

        return AABoundBox{};
    }

private:
    std::unique_ptr<KDTree> m_kdtree;
    std::vector<Hitable*>   m_surfaces_to_add;
};

#endif