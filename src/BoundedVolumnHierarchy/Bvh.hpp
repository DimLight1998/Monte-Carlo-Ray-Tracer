//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_BVH_HPP
#define MONTE_CARLO_RAY_TRACER_BVH_HPP

#include <vector>

#include "../Hitables/HitRecord.hpp"
#include "../Hitables/Hitable.hpp"

class BVH
{
public:
    [[nodiscard]] virtual std::optional<HitRecord> HitBy(const Ray &ray, float tMin, float tMax) const = 0;
    [[nodiscard]] virtual const AlignedBox &GetAlignedBox() const = 0;

    /**
     * @brief Build a BVH from hitables. **Memory should be released manually, and hitables won't preserve order.**
     */
    static const BVH &BuildBVH(const std::vector<std::reference_wrapper<const Hitable>> &hitables);

    /**
     * @brief Recursively delete nodes.
     */
    virtual void Destroy() const = 0;

private:
    static const BVH &BuildBVH(std::vector<std::reference_wrapper<const Hitable>> &hitables, int lo, int hi);
};

#endif //MONTE_CARLO_RAY_TRACER_BVH_HPP
