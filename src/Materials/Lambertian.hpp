//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_LAMBERTIAN_HPP
#define MONTE_CARLO_RAY_TRACER_LAMBERTIAN_HPP

#include "../Textures/Texture.hpp"
#include "../Utils.hpp"
#include "Material.hpp"

class Lambertian : public Material {
  public:
    explicit Lambertian(const std::shared_ptr<Texture> &texture) : _texture{texture} {}

    [[nodiscard]] std::optional<std::pair<Attenuation, Ray>> Scattered(const Ray &ray,
                                                                       const HitRecord &hitRecord) const override {
        const auto scatterDirection = hitRecord.GetNorm() + Utils::RandomPointInUnitSphere();
        const auto scatterRay = Ray(hitRecord.GetLocation(), scatterDirection, ray.GetTimeEmitted());
        const auto attenuation = _texture->GetTextureColorAt(hitRecord.GetUv(), hitRecord.GetLocation());
        return {{attenuation, scatterRay}};
    }

  private:
    const std::shared_ptr<Texture> _texture;
};

#endif // MONTE_CARLO_RAY_TRACER_LAMBERTIAN_HPP
