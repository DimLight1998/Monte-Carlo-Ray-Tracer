//
// Created by zhang on 11/18/19.
//

#include "Ray.hpp"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction, float timeEmitted)
    : _origin { origin }, _direction { direction }, _timeEmitted { timeEmitted } {}
