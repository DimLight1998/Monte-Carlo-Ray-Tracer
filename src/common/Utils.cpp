//
// Created by zhang on 11/18/19.
//

#include "Utils.hpp"

float RandomFloatBetween(float a, float b) {
    static std::mt19937 generator(
        static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_real_distribution<float> distribution(a, b);
    return distribution(generator);
}

float RandomIntegerBetween(int a, int b) {
    static std::mt19937 generator(
        static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(generator);
}

glm::vec3 RandomPointOnUnitDisk() {
    while (true) {
        const auto x = RandomFloatBetween(-1, 1);
        const auto y = RandomFloatBetween(-1, 1);
        glm::vec3  ret(x, y, 0);
        if (glm::length(ret) < 1) {
            return ret;
        }
    }
}

glm::vec3 RandomPointInUnitSphere() {
    while (true) {
        const auto x = RandomFloatBetween(-1, 1);
        const auto y = RandomFloatBetween(-1, 1);
        const auto z = RandomFloatBetween(-1, 1);
        glm::vec3  ret(x, y, z);
        if (glm::length(ret) < 1) {
            return ret;
        }
    }
}

Direction RandomCosineDirection() {
    const auto r1  = RandomFloatBetween(0, 1);
    const auto r2  = RandomFloatBetween(0, 1);
    const auto z   = std::sqrt(1 - r2);
    const auto phi = 2 * Pi * r1;
    const auto x   = std::cos(phi) * 2 * std::sqrt(r2);
    const auto y   = std::sin(phi) * 2 * std::sqrt(r2);
    return { x, y, z };
}
