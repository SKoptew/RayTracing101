#ifndef __KD_TREE_H_INCLUDED
#define __KD_TREE_H_INCLUDED

#include <vector>
#include "Geometry/Hitable.h"

// bound boxes of adjacent nodes can be overlapped => only leaf nodes contains surfaces
class KDTree : public Hitable
{
public:
    enum AXIS
    {
        AXIS_X = 0,
        AXIS_Y = 1,
        AXIS_Z = 2
    };

    KDTree(std::vector<Hitable*> &hitables, size_t start, size_t end);

    bool Hit(const Ray &ray, real t_min, real t_max, HitRecord &hit) const override;
    AABoundBox GetAABoundBox() const override;    

private:
    AABoundBox              m_bbox;  // tighten bound box of objects contained
    AXIS                    m_split_axis;
    std::vector<Hitable*>   m_hitables;    
    std::unique_ptr<KDTree> m_left;
    std::unique_ptr<KDTree> m_right;

    AABoundBox CalcCommonBoundBox(const std::vector<Hitable*> &hitables, size_t start, size_t end) const;
    bool TraverseLeftFirst(const vec3 &ray_direction) const;
    AXIS CalcMaximumDimension(const AABoundBox &bbox) const;

    size_t CalcSplitElement(const std::vector<Hitable*> &hitables, size_t start, size_t end, const AABoundBox &bbox, AXIS split_axis) const;
};

#endif