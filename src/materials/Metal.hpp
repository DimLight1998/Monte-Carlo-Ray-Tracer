//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_METAL_HPP
#define MONTE_CARLO_RAY_TRACER_METAL_HPP

#include "../common/Typing.hpp"
#include "../common/Utils.hpp"
#include "Material.hpp"

class Metal: public Material {
    public:
    Metal(const Attenuation& attenuation, const float fuzziness): _attenuation(attenuation), _fuzziness(fuzziness) {}

    [[nodiscard]] std::optional<ScatterRecord> Scattered(const Ray& ray, const HitRecord& hitRecord) const override {
        const auto reflectDirection = glm::reflect(glm::normalize(ray.GetDirection()), hitRecord.GetNorm());
        const auto scatterDirection = reflectDirection + _fuzziness * RandomPointInUnitSphere();
        return { { _attenuation, nullptr, { { hitRecord.GetLocation(), scatterDirection, ray.GetTimeEmitted() } } } };
    }

    private:
    const Attenuation _attenuation;
    const float       _fuzziness;
};

#endif  // MONTE_CARLO_RAY_TRACER_METAL_HPP
