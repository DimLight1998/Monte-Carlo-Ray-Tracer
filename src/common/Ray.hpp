//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_RAY_HPP
#define MONTE_CARLO_RAY_TRACER_RAY_HPP

#include "Utils.hpp"

#include <glm/glm.hpp>
#include <sstream>

class Ray {
    public:
    Ray(const glm::vec3& origin, const glm::vec3& direction, float timeEmitted);

    [[nodiscard]] inline const glm::vec3& GetOrigin() const {
        return _origin;
    };

    [[nodiscard]] inline const glm::vec3& GetDirection() const {
        return _direction;
    };

    [[nodiscard]] inline float GetTimeEmitted() const {
        return _timeEmitted;
    };

    [[nodiscard]] inline glm::vec3 GetLocationAt(float tDirection) const {
        return _origin + _direction * tDirection;
    };

    private:
    glm::vec3 _origin;
    glm::vec3 _direction;
    float     _timeEmitted;
};

#endif  // MONTE_CARLO_RAY_TRACER_RAY_HPP
