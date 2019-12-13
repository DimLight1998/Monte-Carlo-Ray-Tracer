//
// Created by zhang on 11/19/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_SMOKEWRAPPER_HPP
#define MONTE_CARLO_RAY_TRACER_SMOKEWRAPPER_HPP

#include <memory>

#include "../common/Utils.hpp"
#include "../materials/Smoke.hpp"
#include "../textures/Texture.hpp"
#include "Hitable.hpp"

class SmokeWrapper: public Hitable {
    public:
    SmokeWrapper(const std::shared_ptr<const Hitable>& internalHitable, const std::shared_ptr<Smoke>& smokeMaterial) {
        _hitable       = internalHitable;
        _hitableBVH    = internalHitable->BuildBVH();
        _smokeMaterial = smokeMaterial;
    }

    std::optional<HitRecord> HitBy(const Ray& ray, float tMin, float tMax) const override {
        const auto hitResult1 = _hitableBVH->HitBy(ray, NegInfinity, PosInfinity);
        if (!hitResult1.has_value()) return {};
        const auto& hitRecord1 = hitResult1.value();
        const auto  t1         = hitRecord1.GetT();
        const auto  hitResult2 = _hitableBVH->HitBy(ray, t1 + Epsilon, PosInfinity);
        if (!hitResult2.has_value()) return {};
        const auto& hitRecord2 = hitResult2.value();
        const auto  t2         = hitRecord2.GetT();
        const auto  t1s        = std::max(tMin, t1);
        const auto  t2s        = std::min(tMax, t2);
        if (t1s >= t2s) return {};
        const auto t1p            = std::max(0.0f, t1s);
        const auto distanceToExit = glm::length(((t2s - t1p) * ray.GetDirection()));
        const auto distanceToHit  = (-1.0f / _smokeMaterial->GetDensity()) *
                                   static_cast<float>((log(static_cast<double>(RandomFloatBetween(0, 1)))));
        if (distanceToHit >= distanceToExit) return {};
        const auto hitT = t1p + distanceToHit / glm::length(ray.GetDirection());
        const auto hitP = ray.GetLocationAt(hitT);
        return { { hitP, hitRecord2.GetNorm(), hitRecord2.GetUv(), hitT, _smokeMaterial } };
    }

    AlignedBox GetAlignedBox() const override {
        return _hitable->GetAlignedBox();
    }

    Direction GetRandomRayDirection(const Location& origin) const override {
        return _hitable->GetRandomRayDirection(origin);
    }

    float GetRayPDF(const Location& origin, const Direction& direction) const override {
        return _hitable->GetRayPDF(origin, direction);
    }

    std::unique_ptr<BVH> BuildBVH() const override {
        const auto alignedBox = GetAlignedBox();
        return std::make_unique<BVHLeaf>(alignedBox, shared_from_this());
    }

    std::string ToString() const override {
        return Hitable::ToString();
    }

    virtual Ray GenerateRandomRayForPM() const override {
        return _hitable->GenerateRandomRayForPM();
    }

    private:
    std::shared_ptr<const Hitable> _hitable;
    std::unique_ptr<BVH>           _hitableBVH;
    std::shared_ptr<Smoke>         _smokeMaterial;
};

#endif  //MONTE_CARLO_RAY_TRACER_SMOKEWRAPPER_HPP
