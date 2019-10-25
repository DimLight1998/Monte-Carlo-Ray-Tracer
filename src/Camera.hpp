//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_CAMERA_HPP
#define MONTE_CARLO_RAY_TRACER_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Ray.hpp"
#include "Utils.hpp"

class Camera {
  public:
    Camera(const glm::vec3 &lookFrom, const glm::vec3 &lookAt, const glm::vec3 &upDirection, float verticalFov,
           float aspect, float aperture, float focusDistance, float timeStart, float timeEnd);

    [[nodiscard]] const glm::vec3 &GetPhotoBottomLeftLocation() const;
    [[nodiscard]] const glm::vec3 &GetPhotoLeftRightVec() const;
    [[nodiscard]] const glm::vec3 &GetPhotoBottomTopVec() const;
    [[nodiscard]] const glm::vec3 &GetEyeLocation() const;
    [[nodiscard]] const glm::vec3 &GetU() const;
    [[nodiscard]] const glm::vec3 &GetV() const;
    [[nodiscard]] const glm::vec3 &GetW() const;
    [[nodiscard]] float GetLensRadius() const;
    [[nodiscard]] float GetTimeStart() const;
    [[nodiscard]] float GetTimeEnd() const;

    Ray GetEmittedRay(float s, float t);

  private:
    glm::vec3 _photoBottomLeftLocation{};
    glm::vec3 _photoLeftRightVec{};
    glm::vec3 _photoBottomTopVec{};
    glm::vec3 _eyeLocation{};
    glm::vec3 _u{}; // @inv unit
    glm::vec3 _v{}; // @inv unit
    glm::vec3 _w{}; // @inv unit
    float _lensRadius;
    float _timeStart;
    float _timeEnd;
};

Camera::Camera(const glm::vec3 &lookFrom, const glm::vec3 &lookAt, const glm::vec3 &upDirection, float verticalFov,
               float aspect, float aperture, float focusDistance, float timeStart, float timeEnd) {
    const auto theta = verticalFov * glm::pi<float>() / 180;
    const auto halfHeight = glm::tan(theta / 2);
    const auto halfWidth = aspect * halfHeight;

    _w = glm::normalize(lookFrom - lookAt);
    _u = glm::normalize(glm::cross(upDirection, _w));
    _v = glm::normalize(glm::cross(_w, _u));

    _eyeLocation = lookFrom;
    _photoBottomLeftLocation =
        _eyeLocation - (halfWidth * focusDistance * _u) - (halfHeight * focusDistance * _v) - (focusDistance * _w);
    _photoLeftRightVec = 2 * halfWidth * focusDistance * _u;
    _photoBottomTopVec = 2 * halfHeight * focusDistance * _v;
    _lensRadius = aperture / 2;
    _timeStart = timeStart;
    _timeEnd = timeEnd;
}

Ray Camera::GetEmittedRay(float s, float t) {
    const auto rand = _lensRadius * Utils::RandomPointOnUnitDisk();
    const auto timeEmitted = Utils::RandomFloatBetween(_timeStart, _timeEnd);
    const auto offset = _u * rand.x + _v * rand.y;
    const auto source = _eyeLocation + offset;
    const auto direction = _photoBottomLeftLocation + s * _photoLeftRightVec + t * _photoBottomTopVec - source;
    return {source, direction, timeEmitted};
}

const glm::vec3 &Camera::GetPhotoBottomLeftLocation() const { return _photoBottomLeftLocation; }

const glm::vec3 &Camera::GetPhotoLeftRightVec() const { return _photoLeftRightVec; }

const glm::vec3 &Camera::GetPhotoBottomTopVec() const { return _photoBottomTopVec; }

const glm::vec3 &Camera::GetEyeLocation() const { return _eyeLocation; }

const glm::vec3 &Camera::GetU() const { return _u; }

const glm::vec3 &Camera::GetV() const { return _v; }

const glm::vec3 &Camera::GetW() const { return _w; }

float Camera::GetLensRadius() const { return _lensRadius; }

float Camera::GetTimeStart() const { return _timeStart; }

float Camera::GetTimeEnd() const { return _timeEnd; }

#endif // MONTE_CARLO_RAY_TRACER_CAMERA_HPP
