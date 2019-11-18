//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_TYPING_HPP
#define MONTE_CARLO_RAY_TRACER_TYPING_HPP

#include <glm/glm.hpp>

/*
 * RGB representation of the attenuation. Each component must between 0 and 1.
 */
using Attenuation = glm::vec3;

/*
 * RGB representation of the color. Each component must between 0 and 1.
 */
using Color = glm::vec3;

/*
 * Location (x, y, z). An absolute location in the world.
 */
using Location = glm::vec3;

/*
 * Offset (x, y, z). The difference of two locations.
 */
using Offset = glm::vec3;

/*
 * Direction (x, y, z). Used to represent a direction.
 */
using Direction = glm::vec3;

/*
 * UV coordinate used by textures. Each component must between 0 and 1.
 */
using UVCoordinate = glm::vec2;

#endif  // MONTE_CARLO_RAY_TRACER_TYPING_HPP
