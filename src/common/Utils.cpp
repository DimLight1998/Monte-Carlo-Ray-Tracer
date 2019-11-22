//
// Created by zhang on 11/18/19.
//

#include "Utils.hpp"

float RandomFloatBetween(float a, float b) {
    // todo
//    static std::mt19937 generator(
//        static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
    static std::mt19937 generator(0);
    std::uniform_real_distribution<float> distribution(a, b);
    return distribution(generator);
}

float RandomIntegerBetween(int a, int b) {
    // todo
//    static std::mt19937 generator(
//        static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
    static std::mt19937 generator(0);
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
    const auto x   = std::cos(phi) * std::sqrt(r2);
    const auto y   = std::sin(phi) * std::sqrt(r2);
    return { x, y, z };
}

Direction RandomToSphere(float radius, float distanceSquared) {
    const auto r1  = RandomFloatBetween(0, 1);
    const auto r2  = RandomFloatBetween(0, 1);
    const auto z   = 1 + r2 * (std::sqrt(1 - radius * radius / distanceSquared) - 1);
    const auto phi = 2 * Pi * r1;
    const auto x   = std::cos(phi) * std::sqrt(1 - z * z);
    const auto y   = std::sin(phi) * std::sqrt(1 - z * z);
    return { x, y, z };
}

void NormalizeFloat(glm::vec3& vec) {
    if (std::isnan(vec.x) || std::isinf(vec.x)) vec.x = 0.5;
    if (std::isnan(vec.y) || std::isinf(vec.y)) vec.y = 0.5;
    if (std::isnan(vec.z) || std::isinf(vec.z)) vec.z = 0.5;
}
