#include "h/KDTree.h"

struct CompareByX
{
    bool operator()(Hitable* lhs, Hitable* rhs) const
    {
        return lhs->GetAABoundBox().GetMin().x < rhs->GetAABoundBox().GetMin().x;
    }
};

struct CompareByY
{
    bool operator()(Hitable* lhs, Hitable* rhs) const
    {
        return lhs->GetAABoundBox().GetMin().y < rhs->GetAABoundBox().GetMin().y;
    }
};

struct CompareByZ
{
    bool operator()(Hitable* lhs, Hitable* rhs) const
    {
        return lhs->GetAABoundBox().GetMin().z < rhs->GetAABoundBox().GetMin().z;
    }
};

KDTree::KDTree(std::vector<Hitable*> &hitables, size_t start, size_t end)
{
    m_bbox = CalcCommonBoundBox(hitables, start, end);

    const size_t count = end - start;
    
    if (count > 2) // split hitables to 2 parts
    {
        m_split_axis = CalcMaximumDimension(m_bbox);

        switch (m_split_axis)
        {
        case AXIS_X: std::sort(hitables.begin() + start, hitables.begin() + end, CompareByX()); break;
        case AXIS_Y: std::sort(hitables.begin() + start, hitables.begin() + end, CompareByY()); break;
        case AXIS_Z: std::sort(hitables.begin() + start, hitables.begin() + end, CompareByZ()); break;
        }

        const size_t median = start + count / 2;

        m_left  = std::make_unique<KDTree>(hitables, start, median);
        m_right = std::make_unique<KDTree>(hitables, median, end);
    }
    else // leaf node
    {
        m_hitables.resize(count);

        for (size_t i = start, j = 0; i < end; ++i, ++j)
        {
            m_hitables[j] = hitables[i];
        }
    }
}

bool KDTree::Hit(const Ray &ray, real t_min, real t_max, HitRecord &hit) const
{
    if (m_bbox.Hit(ray, t_min, t_max))
    {
        bool hit_anything = false;
        real t_closest = t_max;

        //-- leaf node
        if (m_hitables.size() > 0)
        {
            for (const auto &node : m_hitables)
            {
                if (node->Hit(ray, t_min, t_closest, hit))
                {
                    hit_anything = true;
                    t_closest = hit.t;
                }
            }
            return hit_anything;
        }
        else
        {
            if (TraverseLeftFirst(ray.direction))
            {
                if (m_left->Hit(ray, t_min, t_closest, hit))
                {
                    hit_anything = true;
                    t_closest = hit.t;
                }

                if (m_right->Hit(ray, t_min, t_closest, hit))
                    hit_anything = true;
            }
            else
            {
                if (m_right->Hit(ray, t_min, t_closest, hit))
                {
                    hit_anything = true;
                    t_closest = hit.t;
                }

                if (m_left->Hit(ray, t_min, t_closest, hit))
                    hit_anything = true;
            }

            return hit_anything;
        }
    }

    return false;
}

AABoundBox KDTree::GetAABoundBox() const
{
    return m_bbox;
}

AABoundBox KDTree::CalcCommonBoundBox(const std::vector<Hitable*> &hitables, size_t start, size_t end) const
{
    AABoundBox bbox = hitables[start]->GetAABoundBox();

    for (size_t i = start+1; i < end; ++i)
    {
        bbox.Expand(hitables[i]->GetAABoundBox());
    }

    return bbox;
}

bool KDTree::TraverseLeftFirst(const vec3 &ray_direction) const
{
    //-- return dot(ray_direction, split_plane_normal) > 0.0;
    switch (m_split_axis)
    {
    case AXIS_X: return ray_direction.x > 0;
    case AXIS_Y: return ray_direction.y > 0;
    case AXIS_Z: return ray_direction.z > 0;
    }

    return false;
}

KDTree::AXIS KDTree::CalcMaximumDimension(const AABoundBox &bbox) const
{
    const auto bbox_size = m_bbox.Size();

    if (bbox_size.x > bbox_size.y)
    {
        return (bbox_size.x > bbox_size.z) ? KDTree::AXIS_X : KDTree::AXIS_Z;
    }
    else
    {
        return (bbox_size.y > bbox_size.z) ? KDTree::AXIS_Y : KDTree::AXIS_Z;
    }
}
