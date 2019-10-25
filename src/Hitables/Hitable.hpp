//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_HITABLE_HPP
#define MONTE_CARLO_RAY_TRACER_HITABLE_HPP

#include "../Ray.hpp"
#include "HitRecord.hpp"
#include "AlignedBox.hpp"
#include "../BoundedVolumnHierarchy/Bvh.hpp"

class BVH;

class Hitable {
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
     * @brief Build the BVH of this hitable. **Should delete it after usage!**
     */
    [[nodiscard]] virtual const BVH& BuildBVH() const = 0;
};

#endif //MONTE_CARLO_RAY_TRACER_HITABLE_HPP
