//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_BVHNODE_HPP
#define MONTE_CARLO_RAY_TRACER_BVHNODE_HPP

#include <iostream>
#include <memory>

#include "../common/AlignedBox.hpp"
#include "Bvh.hpp"

class BVHNode: public BVH {
    public:
    BVHNode(std::unique_ptr<BVH>& bvh1, std::unique_ptr<BVH>& bvh2)
        : _bvh1(std::move(bvh1)),
          _bvh2(std::move(bvh2)),
          _alignedBox { _bvh1->GetAlignedBox() | _bvh2->GetAlignedBox() } {}

    [[nodiscard]] std::optional<HitRecord> HitBy(const Ray& ray, float tMin, float tMax) const override {
        if (_alignedBox.IsHitBy(ray, tMin, tMax)) {
            const auto res1 = _bvh1->HitBy(ray, tMin, tMax);
            const auto res2 = _bvh2->HitBy(ray, tMin, tMax);
            if (!res1 && !res2) return {};
            if (res1 && !res2) return res1;
            if (!res1 && res2) return res2;
            if (res1.value().GetT() > res2.value().GetT())
                return res2;
            else
                return res1;
        } else {
            return {};
        }
    }

    [[nodiscard]] const AlignedBox& GetAlignedBox() const override {
        return _alignedBox;
    }

    virtual std::string ToString() const override {
        return "node " + _alignedBox.ToString() + " [" + _bvh1->ToString() + "] [" + _bvh2->ToString() + "]";
    }

    private:
    const std::unique_ptr<BVH> _bvh1;
    const std::unique_ptr<BVH> _bvh2;
    const AlignedBox           _alignedBox;
};

#endif  // MONTE_CARLO_RAY_TRACER_BVHNODE_HPP
