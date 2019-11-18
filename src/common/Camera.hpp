//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_CAMERA_HPP
#define MONTE_CARLO_RAY_TRACER_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Ray.hpp"
#include "Typing.hpp"
#include "Utils.hpp"

class Camera {
    public:
    Camera(
        const Location&  lookFrom,
        const Location&  lookAt,
        const Direction& upDirection,
        float            verticalFov,
        float            aspect,
        float            aperture,
        float            focusDistance,
        float            shutterStart,
        float            shutterEnd);

    [[nodiscard]] Ray GetEmittedRay(float s, float t) const;

    private:
    glm::vec3 _photoBottomLeftLocation {};
    glm::vec3 _photoLeftRightVec {};
    glm::vec3 _photoBottomTopVec {};
    glm::vec3 _eyeLocation {};
    glm::vec3 _u {};  // @inv unit
    glm::vec3 _v {};  // @inv unit
    glm::vec3 _w {};  // @inv unit
    float     _lensRadius;
    float     _shutterStart;
    float     _shutterEnd;
};

#endif  // MONTE_CARLO_RAY_TRACER_CAMERA_HPP
