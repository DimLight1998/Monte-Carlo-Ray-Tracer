//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_RENDERINGENGINE_HPP
#define MONTE_CARLO_RAY_TRACER_RENDERINGENGINE_HPP

#include <numeric>
#include <vector>

#include "../bounded-volumn-hierarchy/Bvh.hpp"
#include "../common/Typing.hpp"
#include "../hitables/Hitable.hpp"
#include "../hitables/prefabs/Rectangle.hpp"
#include "../pdf/CosinePdf.hpp"
#include "../pdf/HitablePdf.hpp"
#include "../pdf/MixturePDF.hpp"

class RenderingEngine {
    public:
    static Color GetRayColor(const std::unique_ptr<BVH>& bvh, const Ray& ray, int maxDepth, const Color& skyColor) {
        if (maxDepth == 0) return Black;
        const auto hitResult = bvh->HitBy(ray, RayHitMin, RayHitMax);
        if (hitResult) {
            const auto& hitRecord = hitResult.value();
            const auto  scattered = hitRecord.GetMaterial().Scattered(ray, hitRecord);
            const auto  emitted   = hitRecord.GetMaterial().Emitted(hitRecord.GetUv(), hitRecord.GetLocation());
            if (scattered) {
                auto       lightShape        = Rectangle::OfZX(227, 332, 227, 332, 554, false, nullptr);
                const auto lightShapePointer = std::shared_ptr<Mesh>(&lightShape, [](Rectangle* _) {});
                auto       p1                = HitablePDF { lightShapePointer, hitRecord.GetLocation() };
                auto       p2                = CosinePDF { hitRecord.GetNorm() };
                const auto p1p               = std::shared_ptr<PDF>(&p1, [](auto _) {});
                const auto p2p               = std::shared_ptr<PDF>(&p2, [](auto _) {});
                const auto pMix              = MixturePDF { p1p, p2p };
                const auto direction         = pMix.GenerateRayDirection();
                const auto pdfValue          = pMix.GetPDFValue(direction);
                const auto scatteredRay      = Ray { hitRecord.GetLocation(), direction, ray.GetTimeEmitted() };
                const auto attenuation       = scattered.value().GetAttenuation();
                auto       ret               = emitted + attenuation *
                                         hitRecord.GetMaterial().GetScatteringPDF(ray, hitRecord, scatteredRay) *
                                         GetRayColor(bvh, scatteredRay, maxDepth - 1, skyColor) / pdfValue;
                NormalizeFloat(ret);
                return ret;
            } else {
                return emitted;
            }
        } else {
            return skyColor;
        }
    }

    private:
    constexpr static Color Black          = { 0, 0, 0 };
    constexpr static Color DEBUG_ONLY_SKY = { 0.9, 1.0, 1.0 };
    constexpr static float RayHitMin      = Epsilon;
    constexpr static float RayHitMax      = PosInfinity;
};

#endif  // MONTE_CARLO_RAY_TRACER_RENDERINGENGINE_HPP
