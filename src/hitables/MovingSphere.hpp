//
// Created by zhang on 11/26/19.
//

#ifndef MONTECARLORAYTRACER_MOVINGSPHERE_HPP
#define MONTECARLORAYTRACER_MOVINGSPHERE_HPP

#include <memory>

#include "../bounded-volumn-hierarchy/BvhLeaf.hpp"
#include "../common/Geometry.hpp"
#include "../common/Utils.hpp"
#include "../textures/Texture.hpp"
#include "Hitable.hpp"

class MovingSphere: public Hitable {
    public:
    MovingSphere(
        const Location&           centerStart,
        const Location&           centerEnd,
        float                     radius,
        float                     movingStart,
        float                     movingEnd,
        std::shared_ptr<Material> material)
        : _centerStart { centerStart },
          _centerEnd { centerEnd },
          _radius { radius },
          _movingStart { movingStart },
          _movingEnd { movingEnd },
          _material { std::move(material) } {}

    [[nodiscard]] std::optional<HitRecord> HitBy(const Ray& ray, float tMin, float tMax) const override {
        const auto timeEmitted = ray.GetTimeEmitted();
        const auto time        = std::clamp(timeEmitted, _movingStart, _movingEnd);
        const auto lambda      = (time - _movingStart) / (_movingEnd - _movingStart);
        const auto centerNow   = (1 - lambda) * _centerStart + lambda * _centerEnd;

        const auto oc    = ray.GetOrigin() - centerNow;
        const auto a     = glm::dot(ray.GetDirection(), ray.GetDirection());
        const auto b     = glm::dot(oc, ray.GetDirection());
        const auto c     = glm::dot(oc, oc) - _radius * _radius;
        const auto delta = b * b - a * c;

        if (delta <= 0) {
            return {};
        }

        const auto t1 = (-b - glm::sqrt(delta)) / a;
        if (t1 > tMin && t1 < tMax) {
            const auto hitPoint          = ray.GetLocationAt(t1);
            const auto pointOnUnitSphere = (hitPoint - centerNow) / _radius;
            const auto uv                = GetUnitSphereUVOfPoint(pointOnUnitSphere);
            const auto norm              = (hitPoint - centerNow) / _radius;
            return { { hitPoint, norm, uv, t1, _material } };
        }

        const auto t2 = (-b + glm::sqrt(delta)) / a;
        if (t2 > tMin && t2 < tMax) {
            const auto hitPoint          = ray.GetLocationAt(t2);
            const auto pointOnUnitSphere = (hitPoint - centerNow) / _radius;
            const auto uv                = GetUnitSphereUVOfPoint(pointOnUnitSphere);
            const auto norm              = (hitPoint - centerNow) / _radius;
            return { { hitPoint, norm, uv, t2, _material } };
        }

        return {};
    }

    [[nodiscard]] AlignedBox GetAlignedBox() const override {
        const auto xL = std::min(_centerStart.x, _centerEnd.x) - _radius;
        const auto xH = std::max(_centerStart.x, _centerEnd.x) + _radius;
        const auto yL = std::min(_centerStart.y, _centerEnd.y) - _radius;
        const auto yH = std::max(_centerStart.y, _centerEnd.y) + _radius;
        const auto zL = std::min(_centerStart.z, _centerEnd.z) - _radius;
        const auto zH = std::max(_centerStart.z, _centerEnd.z) + _radius;
        return { xL, xH, yL, yH, zL, zH };
    }

    [[nodiscard]] std::unique_ptr<BVH> BuildBVH() const override {
        const auto alignedBox = GetAlignedBox();
        return std::make_unique<BVHLeaf>(alignedBox, shared_from_this());
    }

    virtual Direction GetRandomRayDirection(const Location& origin) const override {
        throw std::runtime_error("MC not support for moving objects");
    }

    virtual float GetRayPDF(const Location& origin, const Direction& direction) const override {
        throw std::runtime_error("MC not support for moving objects");
    }

    virtual Ray GenerateRandomRayForPM() const override {
        throw std::runtime_error("PM not support for moving objects");
    }

    private:
    Location                  _centerStart;
    Location                  _centerEnd;
    float                     _radius;
    float                     _movingStart;
    float                     _movingEnd;
    std::shared_ptr<Material> _material;
};

#endif  //MONTECARLORAYTRACER_MOVINGSPHERE_HPP
