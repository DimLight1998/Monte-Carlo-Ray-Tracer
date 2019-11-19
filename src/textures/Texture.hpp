//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_TEXTURE_HPP
#define MONTE_CARLO_RAY_TRACER_TEXTURE_HPP

#include "../common/Typing.hpp"

class Texture {
    public:
    /**
     * @brief Get the color of the texture at a given point.
     * @param uv UV coordinate of point on the object with this material.
     * @param location Location of the point in the world.
     * @return Color of the texture at given point.
     */
    [[nodiscard]] virtual Color GetTextureColorAt(const UVCoordinate& uv, const Location& location) const = 0;
};

#endif  // MONTE_CARLO_RAY_TRACER_TEXTURE_HPP
