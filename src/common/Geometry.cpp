//
// Created by zhang on 11/18/19.
//

#include "Geometry.hpp"
#include "Utils.hpp"

UVCoordinate GetUnitSphereUVOfPoint(const Location& location) {
    const auto phi   = glm::atan(location.z, location.x);
    const auto theta = glm::asin(location.y);

    const auto pi = Pi;
    return { 1 - (phi + pi) / (2 * pi), (theta + pi / 2) / pi };
}

std::optional<Direction> Refract(const Direction& incident, const Direction& norm, float eta) {
    const auto incidentNormed = glm::normalize(incident);
    const auto dt             = glm::dot(incidentNormed, norm);
    const auto delta          = 1 - eta * eta * (1 - dt * dt);

    if (delta > 0) {
        return { eta * (incidentNormed - norm * dt) - norm * glm::sqrt(delta) };
    } else {
        return {};
    }
}

float Schlick(float cosine, float index) {
    const auto r   = glm::pow(1 - index, 2) / glm::pow(1 + index, 2);
    const auto ret = r + (1 - r) * glm::pow(1 - cosine, 5);
    return ret;
}
