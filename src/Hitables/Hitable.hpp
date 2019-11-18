//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_HITABLE_HPP
#define MONTE_CARLO_RAY_TRACER_HITABLE_HPP

#include "../BoundedVolumnHierarchy/Bvh.hpp"
#include "../common/Ray.hpp"
#include "AlignedBox.hpp"
#include "HitRecord.hpp"

class BVH;

class Hitable : public std::enable_shared_from_this<Hitable> {
  public:
    /**
     * @brief Test hitting with a ray, with `tDirection` limited between tMin and tMax.
     */
    [[nodiscard]] virtual std::optional<HitRecord> HitBy(const Ray &ray, float tMin, float tMax) const = 0;

    /**
     * @brief Get the aligned box of this hitable.
     */
    [[nodiscard]] virtual AlignedBox GetAlignedBox() const = 0;

    /**
     * @brief Build the BVH of this hitable.
     */
    [[nodiscard]] virtual std::unique_ptr<BVH> BuildBVH() const = 0;

    virtual std::string ToString() const { return "hitable"; };
};

#endif // MONTE_CARLO_RAY_TRACER_HITABLE_HPP
