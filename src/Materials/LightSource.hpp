//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_LIGHTSOURCE_HPP
#define MONTE_CARLO_RAY_TRACER_LIGHTSOURCE_HPP

#include "Material.hpp"
#include "../Textures/Texture.hpp"

class LightSource : public Material
{
public:
    explicit LightSource(const Texture &texture) : _texture(texture) {}

    [[nodiscard]] std::optional<std::pair<Attenuation, Ray>> Scattered(const Ray &ray, const HitRecord &hitRecord) const override
    {
        // light doesn't scatter
        return {};
    }

    [[nodiscard]] Color Emitted(const UVCoordinate &uv, const Location &location) const override
    {
        return _texture.GetTextureColorAt(uv, location);
    }

private:
    const Texture &_texture;
};

#endif //MONTE_CARLO_RAY_TRACER_LIGHTSOURCE_HPP
