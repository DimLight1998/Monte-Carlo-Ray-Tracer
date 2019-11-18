//
// Created by zhang on 11/18/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_GEOMETRY_HPP
#define MONTE_CARLO_RAY_TRACER_GEOMETRY_HPP

#include <optional>

#include "Typing.hpp"

/**
 * @brief Given a location on the unit sphere, calculate UV coordinate of it.
 */
UVCoordinate GetUnitSphereUVOfPoint(const Location& location);

std::optional<Direction> Refract(const Direction& incident, const Direction& norm, float eta);

float Schlick(float cosine, float index);

#endif  // MONTE_CARLO_RAY_TRACER_GEOMETRY_HPP
