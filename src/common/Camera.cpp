//
// Created by zhang on 11/18/19.
//

#include "Camera.hpp"

Camera::Camera(
    const Location&  lookFrom,
    const Location&  lookAt,
    const Direction& upDirection,
    float            verticalFov,
    float            aspect,
    float            aperture,
    float            focusDistance,
    float            shutterStart,
    float            shutterEnd) {
    const auto theta      = verticalFov * glm::pi<float>() / 180;
    const auto halfHeight = glm::tan(theta / 2);
    const auto halfWidth  = aspect * halfHeight;

    _w = glm::normalize(lookFrom - lookAt);
    _u = glm::normalize(glm::cross(upDirection, _w));
    _v = glm::normalize(glm::cross(_w, _u));

    _eyeLocation = lookFrom;
    _photoBottomLeftLocation =
        _eyeLocation - (halfWidth * focusDistance * _u) - (halfHeight * focusDistance * _v) - (focusDistance * _w);
    _photoLeftRightVec = 2 * halfWidth * focusDistance * _u;
    _photoBottomTopVec = 2 * halfHeight * focusDistance * _v;
    _lensRadius        = aperture / 2;
    _shutterStart      = shutterStart;
    _shutterEnd        = shutterEnd;
}

Ray Camera::GetEmittedRay(float s, float t) const {
    const auto rand        = _lensRadius * RandomPointOnUnitDisk();
    const auto timeEmitted = RandomFloatBetween(_shutterStart, _shutterEnd);
    const auto offset      = _u * rand.x + _v * rand.y;
    const auto source      = _eyeLocation + offset;
    const auto direction   = _photoBottomLeftLocation + s * _photoLeftRightVec + t * _photoBottomTopVec - source;
    return { source, direction, timeEmitted };
}
