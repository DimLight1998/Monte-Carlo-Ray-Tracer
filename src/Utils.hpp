//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_UTILS_HPP
#define MONTE_CARLO_RAY_TRACER_UTILS_HPP

#include "Aliases.hpp"
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <random>
#include <sstream>

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

    static std::string Vec3ToString(const glm::vec3 &v) {
        std::stringstream ss;
        ss << "{" << v.x << ", " << v.y << ", " << v.z << "}";
        return ss.str();
    }

    static std::pair<float, float> FloatMinMax(float a, float b) {
        return (a < b) ? std::make_pair(a, b) : std::make_pair(b, a);
    }

    static std::optional<Direction> Refract(const Direction &incident, const Direction &norm, float eta) {
        const auto incidentNormed = glm::normalize(incident);
        const auto dt = glm::dot(incidentNormed, norm);
        const auto delta = 1 - eta * eta * (1 - dt * dt);
        if (delta > 0) {
            return {eta * (incidentNormed - norm * dt) - norm * glm::sqrt(delta)};
        } else {
            return {};
        }
    }

    static float Schlick(float cosine, float index) {
        const auto r = glm::pow(1 - index, 2) / glm::pow(1 + index, 2);
        const auto ret = r + (1 - r) * glm::pow(1 - cosine, 5);
        return ret;
    }

    constexpr static float Epsilon = 0.001;
    constexpr static float PosInfinity = std::numeric_limits<float>::max();
    constexpr static float NegInfinity = std::numeric_limits<float>::min();

  private:
};

#endif // MONTE_CARLO_RAY_TRACER_UTILS_HPP
