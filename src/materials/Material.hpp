//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_MATERIAL_HPP
#define MONTE_CARLO_RAY_TRACER_MATERIAL_HPP

#include <optional>
#include <tuple>

#include "../common/Ray.hpp"
#include "../common/Typing.hpp"
#include "../common/ScatteredRay.hpp"
#include "../hitables/HitRecord.hpp"

class HitRecord;

class Material {
    public:
    /**
     * @brief Calculate scattered ray on this material when hit.
     * @param hitRecord Information about the hit location.
     * @return
     *      Empty if there is no scattered ray.
     *      Otherwise, the first component is the attenuation to be applied on the scattered ray,
     *      and the second component is the scattered ray.
     */
    [[nodiscard]] virtual std::optional<ScatteredRay>
    Scattered(const Ray& ray, const HitRecord& hitRecord) const = 0;

    /**
     * @brief Calculate the PDF of the scattered ray for monte carlo method.
     * @param rayIn input ray.
     * @param hitRecord hit record about the input ray.
     * @param rayScattered the scattered ray.
     * @return PDF value of the scattered ray.
     */
    [[nodiscard]] virtual float GetScatteringPDF(const Ray& rayIn, const HitRecord& hitRecord, const Ray& rayScattered) const {
        throw std::runtime_error("not implemented");
    }

    /**
     * @brief Calculate emitted color on this material give location of the point.
     * @param uv UV coordinate of the point.
     * @param location Location of the point in the world.
     * @return Color of the point.
     */
    [[nodiscard]] virtual Color Emitted(const UVCoordinate& uv, const Location& location) const {
        return { 0, 0, 0 };
    };
};

#endif  // MONTE_CARLO_RAY_TRACER_MATERIAL_HPP
