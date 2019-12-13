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
#include "../pdf/Mix2PDF.hpp"
#include "../pdf/Mix3PDF.hpp"
#include "../pdf/MultipleHitablesPDF.hpp"

class RenderingEngine {
    public:
    static Color GetRayColor(
        const std::unique_ptr<BVH>&                        bvh,
        const Ray&                                         ray,
        int                                                maxDepth,
        const Color&                                       skyColor,
        const std::vector<std::shared_ptr<const Hitable>>& mcImportantHitables,
        const PhotonMap&                                   photonMap) {
        if (maxDepth == 0) return Black;
        const auto hitResult = bvh->HitBy(ray, RayHitMin, RayHitMax);
        if (!hitResult) return skyColor;

        const auto& hitRecord = hitResult.value();
        const auto  scattered = hitRecord.GetMaterial().Scattered(ray, hitRecord);
        const auto  emitted   = hitRecord.GetMaterial().Emitted(hitRecord.GetUv(), hitRecord.GetLocation());

        if (scattered && scattered.value().GetMaybeSpecularRay()) {
            // this is a specular ray, MC and PM are overridden
            const auto& scatterRecord = scattered.value();
            const auto  attenuation   = scatterRecord.GetAttenuation();
            const auto& specularRay   = scatterRecord.GetMaybeSpecularRay().value();
            return attenuation * GetRayColor(bvh, specularRay, maxDepth - 1, skyColor, mcImportantHitables, photonMap);
        } else if (scattered) {
            const auto                           location         = hitRecord.GetLocation();
            std::shared_ptr<PDF>                 pdfScatter       = scattered.value().GetPDF();
            std::shared_ptr<MultipleHitablesPDF> pdfHitables      = nullptr;
            std::shared_ptr<PhotonMappingPDF>    pdfPhotonMapping = nullptr;
            std::shared_ptr<PDF>                 pdfFinal         = nullptr;
            if (!mcImportantHitables.empty() && !photonMap.IsEmpty()) {
                // MC, PM and scatter
                pdfHitables      = std::make_shared<MultipleHitablesPDF>(mcImportantHitables, location);
                pdfPhotonMapping = photonMap.GetPDF(hitRecord);
                pdfFinal         = std::make_unique<Mix3PDF>(pdfScatter, pdfHitables, pdfPhotonMapping);
            } else if (!mcImportantHitables.empty()) {
                // only MC and scatter
                pdfPhotonMapping = photonMap.GetPDF(hitRecord);
                pdfFinal         = std::make_unique<Mix2PDF>(pdfScatter, pdfPhotonMapping);
            } else if (!photonMap.IsEmpty()) {
                // only PM and scatter
                pdfHitables = std::make_shared<MultipleHitablesPDF>(mcImportantHitables, location);
                pdfFinal    = std::make_unique<Mix2PDF>(pdfScatter, pdfHitables);
            } else {
                // only scatter
                pdfFinal = pdfScatter;
            }

            const auto direction    = pdfFinal->GenerateRayDirection();
            const auto pdfValue     = pdfFinal->GetPDFValue(direction);
            const auto scatteredRay = Ray { hitRecord.GetLocation(), direction, ray.GetTimeEmitted() };
            const auto attenuation  = scattered.value().GetAttenuation();
            auto       ret =
                emitted + attenuation * hitRecord.GetMaterial().GetScatteringPDF(ray, hitRecord, scatteredRay) *
                              GetRayColor(bvh, scatteredRay, maxDepth - 1, skyColor, mcImportantHitables, photonMap) /
                              pdfValue;
            NormalizeFloat(ret);
            return ret;
        } else {
            return emitted;
        }
    }

    private:
    constexpr static Color Black          = { 0, 0, 0 };
    constexpr static Color DEBUG_ONLY_SKY = { 0.9, 1.0, 1.0 };
    constexpr static float RayHitMin      = Epsilon;
    constexpr static float RayHitMax      = PosInfinity;
};

#endif  // MONTE_CARLO_RAY_TRACER_RENDERINGENGINE_HPP
