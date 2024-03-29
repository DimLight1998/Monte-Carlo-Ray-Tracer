//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_PERLINNOISE_HPP
#define MONTE_CARLO_RAY_TRACER_PERLINNOISE_HPP

#include <algorithm>
#include <vector>

#include "../common/Utils.hpp"
#include "Texture.hpp"

class PerlinNoise: public Texture {
    public:
    PerlinNoise(
        float        scale,
        int          depth,
        float        sinTurbCoeff,
        float        sinOffset,
        float        sinXCoeff,
        float        sinYCoeff,
        float        sinZCoeff,
        float        sinXYCoeff,
        float        sinYZCoeff,
        float        sinZXCoeff,
        float        sinXYZCoeff,
        float        sinCoeff,
        float        linearTurbCoeff,
        float        linearOffset,
        const Color& color)
        : _scale { scale },
          _depth { depth },
          _sinTurbCoeff { sinTurbCoeff },
          _sinOffset { sinOffset },
          _sinXCoeff { sinXCoeff },
          _sinYCoeff { sinYCoeff },
          _sinZCoeff { sinZCoeff },
          _sinXYCoeff { sinXYCoeff },
          _sinYZCoeff { sinYZCoeff },
          _sinZXCoeff { sinZXCoeff },
          _sinXYZCoeff { sinXYZCoeff },
          _sinCoeff { sinCoeff },
          _linearTurbCoeff { linearTurbCoeff },
          _linearOffset { linearOffset },
          _color { color } {
        _randVecs.clear();
        for (auto i = 0; i < NumElements; i++) {
            _randVecs.emplace_back(glm::normalize(
                glm::vec3 { RandomFloatBetween(-1, 1), RandomFloatBetween(-1, 1), RandomFloatBetween(-1, 1) }));
            _permX.emplace_back(i);
            _permY.emplace_back(i);
            _permZ.emplace_back(i);
        }

        const auto seed   = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
        auto       engine = std::default_random_engine(seed);
        std::shuffle(_permX.begin(), _permX.end(), engine);
        std::shuffle(_permX.begin(), _permX.end(), engine);
        std::shuffle(_permX.begin(), _permX.end(), engine);
    }

    [[nodiscard]] Color GetTextureColorAt(const UVCoordinate& uv, const Location& location) const override {
        const auto xD   = [=]() { return _scale * location.x; };
        const auto yD   = [=]() { return _scale * location.y; };
        const auto zD   = [=]() { return _scale * location.z; };
        const auto xyD  = [=]() { return _scale * glm::sqrt(location.x * location.x + location.y * location.y); };
        const auto yzD  = [=]() { return _scale * glm::sqrt(location.y * location.y + location.z * location.z); };
        const auto zxD  = [=]() { return _scale * glm::sqrt(location.z * location.z + location.x * location.x); };
        const auto xyzD = [=]() { return _scale * glm::length(location); };

        const auto turb     = Turb(_scale * location, _depth);
        auto       sinValue = 0.0f;
        sinValue += _sinTurbCoeff * turb;
        sinValue += _sinOffset;
        const auto sinComps = std::vector<std::pair<float, std::function<float(void)>>> {
            { _sinXCoeff, xD },   { _sinYCoeff, yD },   { _sinZCoeff, zD },    { _sinXYCoeff, xyD },
            { _sinYZCoeff, yzD }, { _sinZXCoeff, zxD }, { _sinXYZCoeff, xyzD }
        };
        for (const auto& tp : sinComps) {
            sinValue += tp.first == 0 ? 0 : tp.first * tp.second();
        }
        const auto final = _sinCoeff * std::sin(sinValue) + _linearTurbCoeff * turb + _linearOffset;
        const auto color = _color * final;
        return { std::clamp(color.x, 0.0f, 1.0f), std::clamp(color.y, 0.0f, 1.0f), std::clamp(color.z, 0.0f, 1.0f) };
    }

    private:
    static constexpr unsigned NumElements = 256;
    static constexpr unsigned NEM1        = NumElements - 1;
    const float               _scale;
    const int                 _depth;
    const float               _sinTurbCoeff;
    const float               _sinOffset;
    const float               _sinXCoeff;
    const float               _sinYCoeff;
    const float               _sinZCoeff;
    const float               _sinXYCoeff;
    const float               _sinYZCoeff;
    const float               _sinZXCoeff;
    const float               _sinXYZCoeff;
    const float               _sinCoeff;
    const float               _linearTurbCoeff;
    const float               _linearOffset;
    const Color               _color;
    std::vector<glm::vec3>    _randVecs;
    std::vector<unsigned>     _permX;
    std::vector<unsigned>     _permY;
    std::vector<unsigned>     _permZ;

    [[nodiscard]] float GetNoiseAt(const glm::vec3& p) const {
        const auto u   = p.x - std::floor(p.x);
        const auto v   = p.y - std::floor(p.y);
        const auto w   = p.z - std::floor(p.z);
        const auto i   = static_cast<int>(std::floor(p.x));
        const auto j   = static_cast<int>(std::floor(p.y));
        const auto k   = static_cast<int>(std::floor(p.z));
        const auto ooo = _randVecs[_permX[(i + 0u) & NEM1] ^ _permY[(j + 0u) & NEM1] ^ _permZ[(k + 0u) & NEM1]];
        const auto ooi = _randVecs[_permX[(i + 0u) & NEM1] ^ _permY[(j + 0u) & NEM1] ^ _permZ[(k + 1u) & NEM1]];
        const auto oio = _randVecs[_permX[(i + 0u) & NEM1] ^ _permY[(j + 1u) & NEM1] ^ _permZ[(k + 0u) & NEM1]];
        const auto oii = _randVecs[_permX[(i + 0u) & NEM1] ^ _permY[(j + 1u) & NEM1] ^ _permZ[(k + 1u) & NEM1]];
        const auto ioo = _randVecs[_permX[(i + 1u) & NEM1] ^ _permY[(j + 0u) & NEM1] ^ _permZ[(k + 0u) & NEM1]];
        const auto ioi = _randVecs[_permX[(i + 1u) & NEM1] ^ _permY[(j + 0u) & NEM1] ^ _permZ[(k + 1u) & NEM1]];
        const auto iio = _randVecs[_permX[(i + 1u) & NEM1] ^ _permY[(j + 1u) & NEM1] ^ _permZ[(k + 0u) & NEM1]];
        const auto iii = _randVecs[_permX[(i + 1u) & NEM1] ^ _permY[(j + 1u) & NEM1] ^ _permZ[(k + 1u) & NEM1]];
        return BerlinInterp({ ooo, ooi, oio, oii, ioo, ioi, iio, iii }, u, v, w);
    }

    [[nodiscard]] float Turb(const Location& p, int depth) const {
        auto acc    = 0.0f;
        auto weight = 1.0f;
        auto temp   = p;
        for (auto i = 0; i < depth; i++) {
            acc += weight * GetNoiseAt(temp);
            weight = weight * 0.5;
            temp   = temp * 2.0f;
        }
        return std::abs(acc);
    }

    class ValueCube {
        public:
        ValueCube(
            const glm::vec3& ooo,
            const glm::vec3& ooi,
            const glm::vec3& oio,
            const glm::vec3& oii,
            const glm::vec3& ioo,
            const glm::vec3& ioi,
            const glm::vec3& iio,
            const glm::vec3& iii)
            : OOO(ooo), OOI(ooi), OIO(oio), OII(oii), IOO(ioo), IOI(ioi), IIO(iio), III(iii) {}
        glm::vec3 OOO;
        glm::vec3 OOI;
        glm::vec3 OIO;
        glm::vec3 OII;
        glm::vec3 IOO;
        glm::vec3 IOI;
        glm::vec3 IIO;
        glm::vec3 III;
    };

    [[nodiscard]] static float BerlinInterp(const ValueCube& cube, float u, float v, float w) {
        const auto uu  = u * u * (3 - 2 * u);
        const auto vv  = v * v * (3 - 2 * v);
        const auto ww  = w * w * (3 - 2 * w);
        const auto ooo = glm::dot(cube.OOO, { u - 0, v - 0, w - 0 }) * (1 - uu) * (1 - vv) * (1 - ww);
        const auto ooi = glm::dot(cube.OOI, { u - 0, v - 0, w - 1 }) * (1 - uu) * (1 - vv) * (0 + ww);
        const auto oio = glm::dot(cube.OIO, { u - 0, v - 1, w - 0 }) * (1 - uu) * (0 + vv) * (1 - ww);
        const auto oii = glm::dot(cube.OII, { u - 0, v - 1, w - 1 }) * (1 - uu) * (0 + vv) * (0 + ww);
        const auto ioo = glm::dot(cube.IOO, { u - 1, v - 0, w - 0 }) * (0 + uu) * (1 - vv) * (1 - ww);
        const auto ioi = glm::dot(cube.IOI, { u - 1, v - 0, w - 1 }) * (0 + uu) * (1 - vv) * (0 + ww);
        const auto iio = glm::dot(cube.IIO, { u - 1, v - 1, w - 0 }) * (0 + uu) * (0 + vv) * (1 - ww);
        const auto iii = glm::dot(cube.III, { u - 1, v - 1, w - 1 }) * (0 + uu) * (0 + vv) * (0 + ww);
        return ooo + ooi + oio + oii + ioo + ioi + iio + iii;
    }
};

#endif  // MONTE_CARLO_RAY_TRACER_PERLINNOISE_HPP
