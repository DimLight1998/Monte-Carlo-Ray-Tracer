//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_BVHLEAF_HPP
#define MONTE_CARLO_RAY_TRACER_BVHLEAF_HPP

#include "Bvh.hpp"
#include "../Hitables/AlignedBox.hpp"
#include "../Hitables/Hitable.hpp"

class BVHLeaf : public BVH
{
public:
    BVHLeaf(const AlignedBox &alignedBox, const Hitable &hitable) : _alignedBox(alignedBox), _hitable(hitable) {}

    [[nodiscard]] std::optional<HitRecord> HitBy(const Ray &ray, float tMin, float tMax) const override
    {
        if (_alignedBox.IsHitBy(ray, tMin, tMax))
        {
            return {_hitable.HitBy(ray, tMin, tMax)};
        }
        else
        {
            return {};
        }
    }

    [[nodiscard]] const AlignedBox &GetAlignedBox() const override
    {
        return _alignedBox;
    }

    void Destroy() const override
    {
        delete this;
    }

private:
    const AlignedBox _alignedBox;
    const Hitable &_hitable;
};

#endif //MONTE_CARLO_RAY_TRACER_BVHLEAF_HPP
