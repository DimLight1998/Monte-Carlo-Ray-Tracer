//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_CONSTANTCOLOR_HPP
#define MONTE_CARLO_RAY_TRACER_CONSTANTCOLOR_HPP

#include "Texture.hpp"

class ConstantColor : public Texture
{
public:
    explicit ConstantColor(const Color &color) : _color(color) {}

    [[nodiscard]] Color GetTextureColorAt(const UVCoordinate &uv, const Location &location) const override
    {
        return _color;
    }

private:
    Color _color;
};

#endif //MONTE_CARLO_RAY_TRACER_CONSTANTCOLOR_HPP
