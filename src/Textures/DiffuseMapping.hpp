//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_DIFFUSEMAPPING_HPP
#define MONTE_CARLO_RAY_TRACER_DIFFUSEMAPPING_HPP

#define STB_IMAGE_IMPLEMENTATION

#include <string>

#include <stb_image/stb_image.h>

#include "Texture.hpp"

class DiffuseMapping: public Texture {
    public:
    explicit DiffuseMapping(const std::string& imagePath);
    ~DiffuseMapping();

    [[nodiscard]] Color GetTextureColorAt(const UVCoordinate& uv, const Location& location) const override;

    private:
    int            _width       = 0;
    int            _height      = 0;
    int            _numChannels = 0;
    unsigned char* _data        = nullptr;
};

DiffuseMapping::DiffuseMapping(const std::string& imagePath) {
    stbi_set_flip_vertically_on_load(true);
    _data = stbi_load(imagePath.c_str(), &_width, &_height, &_numChannels, STBI_rgb);
}

DiffuseMapping::~DiffuseMapping() {
    stbi_image_free(_data);
}

Color DiffuseMapping::GetTextureColorAt(const UVCoordinate& uv, const Location& location) const {
    const auto i           = static_cast<int>(uv.x * static_cast<float>(_width));
    const auto j           = static_cast<int>(uv.y * static_cast<float>(_height));
    const auto pixelOffset = _data + (i + _height * j) * _numChannels;
    const auto r           = static_cast<float>(pixelOffset[0]) / 255.0f;
    const auto g           = static_cast<float>(pixelOffset[1]) / 255.0f;
    const auto b           = static_cast<float>(pixelOffset[2]) / 255.0f;

    return { r, g, b };
}

#endif  //MONTE_CARLO_RAY_TRACER_DIFFUSEMAPPING_HPP
