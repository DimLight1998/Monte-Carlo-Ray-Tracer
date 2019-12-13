//
// Created by zhang on 11/20/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_COSINEPDF_HPP
#define MONTE_CARLO_RAY_TRACER_COSINEPDF_HPP

#include <array>

#include "../common/Utils.hpp"
#include "Pdf.hpp"

class OrthoNormalBases {
    public:
    static OrthoNormalBases BuildFromW(const Direction& n) {
        OrthoNormalBases ret {};
        ret._axis[2] = glm::normalize(n);
        Direction a;
        if (std::abs(ret.GetW().x) > 0.9) {
            a = Direction { 0, 1, 0 };
        } else {
            a = Direction { 1, 0, 0 };
        }
        ret._axis[1] = glm::normalize(glm::cross(ret.GetW(), a));
        ret._axis[0] = glm::cross(ret.GetW(), ret.GetV());
        return ret;
    }

    [[nodiscard]] const Direction& GetU() const {
        return _axis[0];
    }

    [[nodiscard]] const Direction& GetV() const {
        return _axis[1];
    }

    [[nodiscard]] const Direction& GetW() const {
        return _axis[2];
    }

    [[nodiscard]] Location GetLocalLocation(float a, float b, float c) const {
        return GetU() * a + GetV() * b + GetW() * c;
    }

    [[nodiscard]] Location GetLocalLocation(const Direction& direction) const {
        return GetU() * direction.x + GetV() * direction.y + GetW() * direction.z;
    }

    private:
    OrthoNormalBases() = default;
    std::array<Direction, 3> _axis;
};

class CosinePDF: public PDF {
    public:
    explicit CosinePDF(const Direction& w);
    [[nodiscard]] float                    GetPDFValue(const Direction& direction) const override;
    [[nodiscard]] std::optional<Direction> GenerateRayDirection() const override;

    private:
    OrthoNormalBases _bases;
};

CosinePDF::CosinePDF(const Direction& w): _bases { OrthoNormalBases::BuildFromW(w) } {}

float CosinePDF::GetPDFValue(const Direction& direction) const {
    const auto cosine = glm::dot(glm::normalize(direction), _bases.GetW());

    return cosine > 0.0f ? (cosine / Pi) : 0.0f;
}

std::optional<Direction> CosinePDF::GenerateRayDirection() const {
    return { _bases.GetLocalLocation(RandomCosineDirection()) };
}

#endif  //MONTE_CARLO_RAY_TRACER_COSINEPDF_HPP
