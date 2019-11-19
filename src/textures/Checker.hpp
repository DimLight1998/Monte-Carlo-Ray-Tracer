//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_CHECKERTEXTURE_HPP
#define MONTE_CARLO_RAY_TRACER_CHECKERTEXTURE_HPP

#include <memory>

#include <glm/glm.hpp>

#include "Texture.hpp"

class Checker: public Texture {
    public:
    Checker(const std::shared_ptr<Texture>& texture1, const std::shared_ptr<Texture>& texture2)
        : _texture1(texture1), _texture2(texture2) {}

    [[nodiscard]] Color GetTextureColorAt(const UVCoordinate& uv, const Location& location) const override {
        if (glm::sin(10 * location.x) + glm::sin(10 * location.y) + glm::sin(10 * location.z) < 0) {
            return _texture1->GetTextureColorAt(uv, location);
        } else {
            return _texture2->GetTextureColorAt(uv, location);
        }
    }

    private:
    const std::shared_ptr<Texture> _texture1;
    const std::shared_ptr<Texture> _texture2;
};

#endif  // MONTE_CARLO_RAY_TRACER_CHECKERTEXTURE_HPP
