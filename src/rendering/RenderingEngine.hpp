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
                //                const auto onLight                 = glm::vec3 { 213 + RandomFloatBetween(0, 1) * (343 - 213),
                //                                                 554,
                //                                                 227 + RandomFloatBetween(0, 1) * (332 - 227) };
                //                const auto toLight                 = onLight - hitRecord.GetLocation();
                //                const auto hitLightDistance        = glm::length(toLight);
                //                const auto hitLightDistanceSquared = hitLightDistance * hitLightDistance;  // todo optimization
                //                const auto unitToLight             = glm::normalize(toLight);
                //                if (glm::dot(unitToLight, hitRecord.GetNorm()) < 0) return emitted;
                //                const auto lightArea   = (343 - 213) * (332 - 227);
                //                const auto lightCosine = abs(unitToLight.y);
                //                if (lightCosine < 0.000001) return emitted;
                //                const auto pdf          = hitLightDistanceSquared / (lightCosine * lightArea);
                //                const auto scatteredRay = Ray { hitRecord.GetLocation(), unitToLight, ray.GetTimeEmitted() };
                //

                auto       lightShape        = Rectangle::OfZX(227, 332, 227, 332, 554, false, nullptr);
                const auto lightShapePointer = std::shared_ptr<Mesh>(&lightShape, [](Rectangle* _) {});
                HitablePDF p(lightShapePointer, hitRecord.GetLocation());

                //                CosinePDF  p(hitRecord.GetNorm());
                const auto generatedWithPDF = p.GenerateWithPDF();
                const auto direction        = generatedWithPDF.first;
                const auto pdfValue         = generatedWithPDF.second;
                const auto scatteredRay     = Ray { hitRecord.GetLocation(), direction, ray.GetTimeEmitted() };
                const auto attenuation      = scattered.value().GetAttenuation();
                //                const auto scatteredRay = scattered.value().GetRay();
                return emitted + attenuation * hitRecord.GetMaterial().GetScatteringPDF(ray, hitRecord, scatteredRay) *
                                     GetRayColor(bvh, scatteredRay, maxDepth - 1, skyColor) / pdfValue;
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
