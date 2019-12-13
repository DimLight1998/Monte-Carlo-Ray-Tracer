//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_UTILS_HPP
#define MONTE_CARLO_RAY_TRACER_UTILS_HPP

#include <chrono>
#include <optional>
#include <random>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Typing.hpp"

class Utils {
    public:
    private:
};

/**
 * @brief Return a random float in a given range.
 * @param a Lower bound.
 * @param b Upper bound.
 * @return A random float in [a, b].
 */
float RandomFloatBetween(float a, float b);

/**
 * @brief Return a random integer in a given range.
 * @param a Lower bound.
 * @param b Upper bound.
 * @return A random integer in [a, b].
 */
float RandomIntegerBetween(int a, int b);

/**
 * @brief Get a random point on the unit disk with z = 0.
 * @return A random point on the unit disk with z = 0.
 */
glm::vec3 RandomPointOnUnitDisk();
;

/**
 * @brief Get a random point in the unit sphere.
 * @return A random point in the unit sphere.
 */
glm::vec3 RandomPointInUnitSphere();

/**
 * @brief Get a random point on the unit sphere.
 * @return A random point on the unit sphere.
 */
glm::vec3 RandomPointOnUnitSphere();

Direction RandomCosineDirection();

Direction RandomToSphere(float radius, float distanceSquared);

void NormalizeFloat(glm::vec3& vec);

constexpr static float Epsilon     = 0.001;
constexpr static float Pi          = glm::pi<float>();
constexpr static float PosInfinity = std::numeric_limits<float>::max();
constexpr static float NegInfinity = std::numeric_limits<float>::min();

#endif  // MONTE_CARLO_RAY_TRACER_UTILS_HPP
