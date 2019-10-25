//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_SPHERE_HPP
#define MONTE_CARLO_RAY_TRACER_SPHERE_HPP

#include "Hitable.hpp"
#include "../Utils.hpp"
#include "../BoundedVolumnHierarchy/BvhLeaf.hpp"

class Sphere : public Hitable {
public:
    Sphere(const Location &center, const float radius, const Material &material) : _center(center), _radius(radius),
                                                                                   _material(material) {}

    [[nodiscard]] const Location &GetCenter() const {
        return _center;
    }

    [[nodiscard]] float GetRadius() const {
        return _radius;
    }

    [[nodiscard]] const Material &GetMaterial() const {
        return _material;
    }

    [[nodiscard]] std::optional<HitRecord> HitBy(const Ray &ray, float tMin, float tMax) const override {
        const auto oc = ray.GetOrigin() - _center;
        const auto a = glm::dot(ray.GetDirection(), ray.GetDirection());
        const auto b = glm::dot(oc, ray.GetDirection());
        const auto c = glm::dot(oc, oc) - _radius * _radius;
        const auto delta = b * b - a * c;

        if (delta <= 0) {
            return {};
        }

        const auto t1 = (-b - glm::sqrt(delta)) / a;
        if (t1 > tMin && t1 < tMax) {
            const auto hitPoint = ray.GetLocationAt(t1);
            const auto pointOnUnitSphere = (hitPoint - _center) / _radius;
            const auto uv = Utils::GetUnitSphereUVOfPoint(pointOnUnitSphere);
            const auto norm = (hitPoint - _center) / _radius;
            return {{hitPoint, norm, uv, t1, _material}};
        }

        const auto t2 = (-b + glm::sqrt(delta)) / a;
        if (t2 > tMin && t2 < tMax) {
            const auto hitPoint = ray.GetLocationAt(t2);
            const auto pointOnUnitSphere = (hitPoint - _center) / _radius;
            const auto uv = Utils::GetUnitSphereUVOfPoint(pointOnUnitSphere);
            const auto norm = (hitPoint - _center) / _radius;
            return {{hitPoint, norm, uv, t2, _material}};
        }

        return {};
    }

    [[nodiscard]] AlignedBox GetAlignedBox() const override {
        const auto ls = _center - _radius;
        const auto hs = _center + _radius;
        return AlignedBox(ls.x, hs.x, ls.y, hs.y, ls.z, hs.z);
    }

    [[nodiscard]] const BVH &BuildBVH() const override {
        const auto alignedBox = GetAlignedBox();
        return *new BVHLeaf(alignedBox, *this);
    }

private:
    const Location _center;
    const float _radius;
    const Material &_material;
};

#endif //MONTE_CARLO_RAY_TRACER_SPHERE_HPP
