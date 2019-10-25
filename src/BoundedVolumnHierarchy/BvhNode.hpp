//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_BVHNODE_HPP
#define MONTE_CARLO_RAY_TRACER_BVHNODE_HPP

#include "Bvh.hpp"
#include "../Hitables/AlignedBox.hpp"

class BVHNode : public BVH
{
public:
    BVHNode(const BVH &bvh1, const BVH &bvh2)
        : _bvh1(bvh1), _bvh2(bvh2), _alignedBox{bvh1.GetAlignedBox() | bvh2.GetAlignedBox()} {}

    [[nodiscard]] std::optional<HitRecord> HitBy(const Ray &ray, float tMin, float tMax) const override
    {
        if (_alignedBox.IsHitBy(ray, tMin, tMax))
        {
            const auto res1 = _bvh1.HitBy(ray, tMin, tMax);
            const auto res2 = _bvh2.HitBy(ray, tMin, tMax);
            if (!res1 && !res2)
                return {};
            if (res1 && !res2)
                return res1;
            if (!res1 && res2)
                return res2;
            if (res1.value().GetT() > res2.value().GetT())
                return res2;
            else
                return res1;
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
        _bvh1.Destroy();
        _bvh2.Destroy();
        delete this;
    }

private:
    const AlignedBox _alignedBox;
    const BVH &_bvh1;
    const BVH &_bvh2;
};

#endif //MONTE_CARLO_RAY_TRACER_BVHNODE_HPP
