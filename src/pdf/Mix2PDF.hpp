//
// Created by zhang on 11/21/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_MIX2PDF_HPP
#define MONTE_CARLO_RAY_TRACER_MIX2PDF_HPP

#include <memory>

#include "../common/Utils.hpp"
#include "Pdf.hpp"

class Mix2PDF: public PDF {
    public:
    [[deprecated]] Mix2PDF(std::shared_ptr<PDF> pdf1, std::shared_ptr<PDF> pdf2)
        : _pdf1 { std::move(pdf1) }, _pdf2 { std::move(pdf2) } {}

    [[nodiscard]] float GetPDFValue(const Direction& direction) const override;

    [[nodiscard]] std::optional<Direction> GenerateRayDirection() const override;

    private:
    std::shared_ptr<PDF> _pdf1;
    std::shared_ptr<PDF> _pdf2;
};

float Mix2PDF::GetPDFValue(const Direction& direction) const {
    return 0.5f * _pdf1->GetPDFValue(direction) + 0.5f * _pdf2->GetPDFValue(direction);
}

std::optional<Direction> Mix2PDF::GenerateRayDirection() const {
    const auto decide = RandomFloatBetween(0, 1);
    if (decide > 0.5f) {
        return _pdf1->GenerateRayDirection();
    } else {
        return _pdf2->GenerateRayDirection();
    }
}

#endif  //MONTE_CARLO_RAY_TRACER_MIX2PDF_HPP
