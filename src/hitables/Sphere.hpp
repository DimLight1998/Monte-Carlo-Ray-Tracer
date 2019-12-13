//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_SPHERE_HPP
#define MONTE_CARLO_RAY_TRACER_SPHERE_HPP

#include "../bounded-volumn-hierarchy/BvhLeaf.hpp"
#include "../common/Geometry.hpp"
#include "../common/Utils.hpp"
#include "Hitable.hpp"

class Sphere: public Hitable {
    public:
    Sphere(const Location& center, const float radius, const std::shared_ptr<Material>& material)
        : _center(center), _radius(radius), _material(material) {}

    [[nodiscard]] const Location& GetCenter() const {
        return _center;
    }

    [[nodiscard]] float GetRadius() const {
        return _radius;
    }

    [[nodiscard]] const Material& GetMaterial() const {
        return *_material;
    }

    [[nodiscard]] std::optional<HitRecord> HitBy(const Ray& ray, float tMin, float tMax) const override {
        const auto oc    = ray.GetOrigin() - _center;
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
            const auto pointOnUnitSphere = (hitPoint - _center) / _radius;
            const auto uv                = GetUnitSphereUVOfPoint(pointOnUnitSphere);
            const auto norm              = (hitPoint - _center) / _radius;
            return { { hitPoint, norm, uv, t1, _material } };
        }

        const auto t2 = (-b + glm::sqrt(delta)) / a;
        if (t2 > tMin && t2 < tMax) {
            const auto hitPoint          = ray.GetLocationAt(t2);
            const auto pointOnUnitSphere = (hitPoint - _center) / _radius;
            const auto uv                = GetUnitSphereUVOfPoint(pointOnUnitSphere);
            const auto norm              = (hitPoint - _center) / _radius;
            return { { hitPoint, norm, uv, t2, _material } };
        }

        return {};
    }

    [[nodiscard]] AlignedBox GetAlignedBox() const override {
        const auto ls = _center - _radius;
        const auto hs = _center + _radius;
        return AlignedBox(ls.x, hs.x, ls.y, hs.y, ls.z, hs.z);
    }

    [[nodiscard]] std::unique_ptr<BVH> BuildBVH() const override {
        const auto alignedBox = GetAlignedBox();
        return std::make_unique<BVHLeaf>(alignedBox, shared_from_this());
    }

    virtual Direction GetRandomRayDirection(const Location& origin) const override {
        const auto direction       = _center - origin;
        const auto distanceSquared = glm::length2(direction);
        const auto bases           = OrthoNormalBases::BuildFromW(direction);
        const auto randomDirection = RandomToSphere(_radius, distanceSquared);
        return bases.GetLocalLocation(randomDirection);
    }

    virtual float GetRayPDF(const Location& origin, const Direction& direction) const override {
        const auto maybeHitRecord = HitBy({ origin, direction, 0 }, Epsilon, PosInfinity);
        if (!maybeHitRecord) return 0;
        const auto cosThetaMax = std::sqrt(1 - _radius * _radius / glm::length2(_center - origin));
        const auto solidAngle  = 2 * Pi * (1 - cosThetaMax);
        return 1 / solidAngle;
    }

    virtual std::string ToString() const override {
        std::stringstream ss;
        ss << "sphere of " << _radius;
        return ss.str();
    }

    virtual Ray GenerateRandomRayForPM() const override {
        const auto direction = RandomPointOnUnitSphere();
        const auto origin    = _center + (_radius + Epsilon) * direction;
        return { origin, direction, 0 };
    }

    private:
    const Location                  _center;
    const float                     _radius;
    const std::shared_ptr<Material> _material;
};

#endif  // MONTE_CARLO_RAY_TRACER_SPHERE_HPP
