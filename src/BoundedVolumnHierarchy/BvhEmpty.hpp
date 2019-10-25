//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_BVHEMPTY_HPP
#define MONTE_CARLO_RAY_TRACER_BVHEMPTY_HPP

#include "../Ray.hpp"
#include "Bvh.hpp"
#include <optional>

class BVH;

class BVHEmpty : public BVH {
  public:
    [[nodiscard]] std::optional<HitRecord> HitBy(const Ray &ray, float tMin, float tMax) const override { return {}; }

    [[nodiscard]] const AlignedBox &GetAlignedBox() const override {
        throw std::runtime_error("trying to get aligned box of BVHEmpty");
    }

    virtual std::string ToString() const override { return "empty"; }
};

#endif // MONTE_CARLO_RAY_TRACER_BVHEMPTY_HPP
