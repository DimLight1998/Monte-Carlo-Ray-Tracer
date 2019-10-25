//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_SMOKE_HPP
#define MONTE_CARLO_RAY_TRACER_SMOKE_HPP

#include "../Textures/Texture.hpp"
#include "../Utils.hpp"
#include "Material.hpp"

class Smoke : public Material {
  public:
    explicit Smoke(const std::shared_ptr<Texture> &texture) : _texture(texture) {}

    [[nodiscard]] std::optional<std::pair<Attenuation, Ray>> Scattered(const Ray &ray,
                                                                       const HitRecord &hitRecord) const override {
        const auto attenuation = _texture->GetTextureColorAt(hitRecord.GetUv(), hitRecord.GetLocation());
        const auto scatterDirection = Utils::RandomPointInUnitSphere();
        return {{attenuation, Ray(hitRecord.GetLocation(), scatterDirection, ray.GetTimeEmitted())}};
    }

  private:
    const std::shared_ptr<Texture> &_texture;
};

#endif // MONTE_CARLO_RAY_TRACER_SMOKE_HPP
