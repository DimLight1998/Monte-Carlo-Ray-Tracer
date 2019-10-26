//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_GLASS_HPP
#define MONTE_CARLO_RAY_TRACER_GLASS_HPP

#include "Material.hpp"

class Glass : public Material {
  public:
    Glass(const float refractIndex, const Attenuation &attenuation)
        : _refractIndex(refractIndex), _attenuation(attenuation) {}

    [[nodiscard]] std::optional<std::pair<Attenuation, Ray>> Scattered(const Ray &ray,
                                                                       const HitRecord &hitRecord) const override {
        const auto toOuter = glm::dot(ray.GetDirection(), hitRecord.GetNorm()) > 0;
        const auto normSide = toOuter ? (-hitRecord.GetNorm()) : hitRecord.GetNorm();
        const auto eta = toOuter ? _refractIndex : 1 / _refractIndex;
        const auto cosine =
            toOuter
                ? (glm::dot(ray.GetDirection(), hitRecord.GetNorm()) / glm::length(ray.GetDirection())) * _refractIndex
                : (glm::dot(ray.GetDirection(), hitRecord.GetNorm()) / glm::length(ray.GetDirection())) * (-1);
        const auto res = Utils::Refract(ray.GetDirection(), normSide, eta);
        if (res) {
            const auto refracted = res.value();
            const auto threshold = Utils::Schlick(cosine, _refractIndex);
            const auto value = Utils::RandomFloatBetween(0, 1);
            if (value < threshold) {
                const auto reflected = glm::reflect(ray.GetDirection(), hitRecord.GetNorm());
                return {{{1, 1, 1}, {hitRecord.GetLocation(), reflected, ray.GetTimeEmitted()}}};
            } else {
                return {{_attenuation, {hitRecord.GetLocation(), refracted, ray.GetTimeEmitted()}}};
            }
        } else {
            const auto reflected = glm::reflect(ray.GetDirection(), hitRecord.GetNorm());
            return {{{1, 1, 1}, {hitRecord.GetLocation(), reflected, ray.GetTimeEmitted()}}};
        }
    }

  private:
    const float _refractIndex;
    const Attenuation _attenuation;
};

#endif // MONTE_CARLO_RAY_TRACER_GLASS_HPP
