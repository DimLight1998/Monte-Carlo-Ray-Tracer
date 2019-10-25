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
        // todo
        const auto a = glm::refract(ray.GetDirection(), hitRecord.GetNorm(), _refractIndex);
        // todo add schlick
        return {};
    }

  private:
    const float _refractIndex;
    const Attenuation _attenuation;
};

#endif // MONTE_CARLO_RAY_TRACER_GLASS_HPP
