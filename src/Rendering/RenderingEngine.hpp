//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_RENDERINGENGINE_HPP
#define MONTE_CARLO_RAY_TRACER_RENDERINGENGINE_HPP

#include <numeric>
#include <vector>

#include "../Aliases.hpp"
#include "../BoundedVolumnHierarchy/Bvh.hpp"
#include "../Hitables/Hitable.hpp"

class RenderingEngine {
  public:
    static Color GetRayColor(const std::unique_ptr<BVH> &bvh, const Ray &ray, int maxDepth) {
        if (maxDepth == 0)
            return Black;
        const auto hitResult = bvh->HitBy(ray, RayHitMin, RayHitMax);
        if (hitResult) {
            const auto &hitRecord = hitResult.value();
            const auto scattered = hitRecord.GetMaterial().Scattered(ray, hitRecord);
            const auto emitted = hitRecord.GetMaterial().Emitted(hitRecord.GetUv(), hitRecord.GetLocation());
            if (scattered) {
                return emitted + scattered.value().first * GetRayColor(bvh, scattered.value().second, maxDepth - 1);
            } else {
                return emitted;
            }
        } else {
            return DEBUG_ONLY_SKY;
        }
    }

  private:
    constexpr static int DEBUG_ONLY_RENDER_DEPTH = 10;
    constexpr static Color Black = {0, 0, 0};
    constexpr static Color DEBUG_ONLY_SKY = {0.9, 1.0, 1.0};
    //    constexpr static Color DEBUG_ONLY_SKY = {0.1, 0.1, 0.1};
    constexpr static float RayHitMin = 1e-3;
    constexpr static float RayHitMax = std::numeric_limits<float>::max();
};

#endif // MONTE_CARLO_RAY_TRACER_RENDERINGENGINE_HPP
