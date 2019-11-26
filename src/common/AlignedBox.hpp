//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_ALIGNEDBOX_HPP
#define MONTE_CARLO_RAY_TRACER_ALIGNEDBOX_HPP

#include <algorithm>
#include <iostream>
#include <sstream>

#include "Ray.hpp"
#include "Utils.hpp"

class AlignedBox {
    public:
    AlignedBox() {
        const auto max = PosInfinity;
        const auto min = -NegInfinity;
        _xMin          = max;
        _xMax          = min;
        _yMin          = max;
        _yMax          = min;
        _zMin          = max;
        _zMax          = min;
    }

    AlignedBox(
        const float xMin,
        const float xMax,
        const float yMin,
        const float yMax,
        const float zMin,
        const float zMax)
        : _xMin(xMin), _xMax(xMax), _yMin(yMin), _yMax(yMax), _zMin(zMin), _zMax(zMax) {}

    AlignedBox& operator=(const AlignedBox&) = default;

    const float GetXMin() const {
        return _xMin;
    }

    const float GetXMax() const {
        return _xMax;
    }

    const float GetYMin() const {
        return _yMin;
    }

    const float GetYMax() const {
        return _yMax;
    }

    const float GetZMin() const {
        return _zMin;
    }

    const float GetZMax() const {
        return _zMax;
    }

    bool IsHitBy(const Ray& ray, float tMin, float tMax) const {
        const auto& origin        = ray.GetOrigin();
        const auto& direction     = ray.GetDirection();
        const auto  updateAndTest = [&tMin, &tMax](float min, float max, float origin, float direction) -> bool {
            const auto minVal = (min - origin) / direction;
            const auto maxVal = (max - origin) / direction;
            const auto minMax = std::minmax(minVal, maxVal);
            tMin              = std::max(minMax.first, tMin);
            tMax              = std::min(minMax.second, tMax);
            return tMin < tMax;
        };
        return updateAndTest(_xMin, _xMax, origin.x, direction.x) &&
               updateAndTest(_yMin, _yMax, origin.y, direction.y) && updateAndTest(_zMin, _zMax, origin.z, direction.z);
    }

    friend AlignedBox operator|(const AlignedBox& left, const AlignedBox& right) {
        return AlignedBox(
            std::min(left.GetXMin(), right.GetXMin()),
            std::max(left.GetXMax(), right.GetXMax()),
            std::min(left.GetYMin(), right.GetYMin()),
            std::max(left.GetYMax(), right.GetYMax()),
            std::min(left.GetZMin(), right.GetZMin()),
            std::max(left.GetZMax(), right.GetZMax()));
    }

    std::string ToString() const {
        std::stringstream ss;
        ss << "(" << _xMin << " " << _xMax << " " << _yMin << " " << _yMax << " " << _zMin << " " << _zMax << ")";
        return ss.str();
    }

    private:
    float _xMin;
    float _xMax;
    float _yMin;
    float _yMax;
    float _zMin;
    float _zMax;
};

#endif  // MONTE_CARLO_RAY_TRACER_ALIGNEDBOX_HPP
