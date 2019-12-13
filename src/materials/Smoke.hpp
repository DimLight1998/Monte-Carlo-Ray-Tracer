//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_SMOKE_HPP
#define MONTE_CARLO_RAY_TRACER_SMOKE_HPP

#include "../common/Utils.hpp"
#include "../textures/Texture.hpp"
#include "Material.hpp"


class Smoke: public Material {
    public:
    explicit Smoke(const std::shared_ptr<Texture>& texture, float density)
        : _density { density }, _texture { texture } {}

    [[nodiscard]] std::optional<ScatterRecord> Scattered(const Ray& ray, const HitRecord& hitRecord) const override {
        const auto attenuation      = _texture->GetTextureColorAt(hitRecord.GetUv(), hitRecord.GetLocation());
        const auto scatterDirection = RandomPointInUnitSphere();
        return { { attenuation, nullptr, { { hitRecord.GetLocation(), scatterDirection, ray.GetTimeEmitted() } } } };
    }

    [[nodiscard]] float GetDensity() const {
        return _density;
    }

    private:
    const float                    _density;
    const std::shared_ptr<Texture> _texture;
};

#endif  // MONTE_CARLO_RAY_TRACER_SMOKE_HPP
