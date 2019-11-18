//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_BVHLEAF_HPP
#define MONTE_CARLO_RAY_TRACER_BVHLEAF_HPP

#include "../Hitables/AlignedBox.hpp"
#include "../Hitables/Hitable.hpp"
#include "Bvh.hpp"

class BVHLeaf: public BVH {
    public:
    BVHLeaf(const AlignedBox& alignedBox, const std::shared_ptr<const Hitable>& hitable)
        : _alignedBox(alignedBox), _hitable(hitable) {}

    [[nodiscard]] std::optional<HitRecord> HitBy(const Ray& ray, float tMin, float tMax) const override {
        if (_alignedBox.IsHitBy(ray, tMin, tMax)) {
            return { _hitable->HitBy(ray, tMin, tMax) };
        } else {
            return {};
        }
    }

    [[nodiscard]] const AlignedBox& GetAlignedBox() const override {
        return _alignedBox;
    }

    virtual std::string ToString() const override {
        return "leaf " + _alignedBox.ToString() + " <" + _hitable->ToString() + ">";
    }

    private:
    const std::shared_ptr<const Hitable> _hitable;
    const AlignedBox                     _alignedBox;
};

#endif  // MONTE_CARLO_RAY_TRACER_BVHLEAF_HPP
