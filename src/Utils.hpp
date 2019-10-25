//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_UTILS_HPP
#define MONTE_CARLO_RAY_TRACER_UTILS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <random>
#include <chrono>
#include "Aliases.hpp"

class Utils {
public:
    /**
     * @brief Return a random float in a given range.
     * @param a Lower bound.
     * @param b Upper bound.
     * @return A random float in [a, b].
     */
    static float RandomFloatBetween(float a, float b) {
        static std::mt19937 generator(
            static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::uniform_real_distribution<float> distribution(a, b);
        return distribution(generator);
    }

    /**
     * @brief Return a random integer in a given range.
     * @param a Lower bound.
     * @param b Upper bound.
     * @return A random integer in [a, b].
     */
    static float RandomIntBetween(int a, int b) {
        static std::mt19937 generator(
            static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution<int> distribution(a, b);
        return distribution(generator);
    }

    /**
     * @brief Get a random point on the unit disk with z = 0.
     * @return A random point on the unit disk with z = 0.
     */
    static glm::vec3 RandomPointOnUnitDisk() {
        while (true) {
            const auto x = RandomFloatBetween(-1, 1);
            const auto y = RandomFloatBetween(-1, 1);
            glm::vec3 ret(x, y, 0);
            if (glm::length(ret) < 1) {
                return ret;
            }
        }
    };

    /**
     * @brief Get a random point in the unit sphere.
     * @return A random point in the unit sphere.
     */
    static glm::vec3 RandomPointInUnitSphere() {
        while (true) {
            const auto x = RandomFloatBetween(-1, 1);
            const auto y = RandomFloatBetween(-1, 1);
            const auto z = RandomFloatBetween(-1, 1);
            glm::vec3 ret(x, y, z);
            if (glm::length(ret) < 1) {
                return ret;
            }
        }
    }

    /**
     * @brief Given a location on the unit sphere, calculate UV coordinate of it.
     */
    static UVCoordinate GetUnitSphereUVOfPoint(const Location &location) {
        const auto phi = glm::atan(location.z, location.x);
        const auto theta = glm::asin(location.y);

        const auto pi = glm::pi<float>();
        return {1 - (phi + pi) / (2 * pi), (theta + pi / 2) / pi};
    }

private:
};

#endif //MONTE_CARLO_RAY_TRACER_UTILS_HPP
