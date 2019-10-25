//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_RAY_HPP
#define MONTE_CARLO_RAY_TRACER_RAY_HPP

#include <glm/glm.hpp>

class Ray {
  public:
    Ray(const glm::vec3 &origin, const glm::vec3 &direction, float timeEmitted)
        : _origin{origin}, _direction{direction}, _timeEmitted{timeEmitted} {};

    [[nodiscard]] const glm::vec3 &GetOrigin() const { return _origin; };

    [[nodiscard]] const glm::vec3 &GetDirection() const { return _direction; };

    [[nodiscard]] float GetTimeEmitted() const { return _timeEmitted; };

    [[nodiscard]] glm::vec3 GetLocationAt(float tDirection) const { return _origin + _direction * tDirection; };

  private:
    glm::vec3 _origin;
    glm::vec3 _direction;
    float _timeEmitted;
};

#endif // MONTE_CARLO_RAY_TRACER_RAY_HPP
