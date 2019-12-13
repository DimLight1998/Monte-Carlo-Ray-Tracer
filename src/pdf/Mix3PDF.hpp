//
// Created by zhang on 12/13/19.
//

#ifndef MONTECARLORAYTRACER_MIX3PDF_HPP
#define MONTECARLORAYTRACER_MIX3PDF_HPP

#include <memory>

#include "../common/Utils.hpp"
#include "Pdf.hpp"

class Mix3PDF: public PDF {
    public:
    Mix3PDF(std::shared_ptr<PDF> pdf1, std::shared_ptr<PDF> pdf2, std::shared_ptr<PDF> pdf3)
        : _pdf1 { std::move(pdf1) }, _pdf2 { std::move(pdf2) }, _pdfFallback { std::move(pdf3) } {}

    [[nodiscard]] float GetPDFValue(const Direction& direction) const override;

    [[nodiscard]] std::optional<Direction> GenerateRayDirection() const override;

    private:
    std::shared_ptr<PDF> _pdf1;
    std::shared_ptr<PDF> _pdf2;
    std::shared_ptr<PDF> _pdfFallback;
};

float Mix3PDF::GetPDFValue(const Direction& direction) const {
    const auto sum =
        _pdf1->GetPDFValue(direction) + _pdf2->GetPDFValue(direction) + _pdfFallback->GetPDFValue(direction);
    return sum / 3;
}

std::optional<Direction> Mix3PDF::GenerateRayDirection() const {
    const auto dice = RandomIntegerBetween(0, 2);
    if (dice == 0) {
        const auto pdf1Res = _pdf1->GenerateRayDirection();
        return pdf1Res ? pdf1Res : _pdfFallback->GenerateRayDirection();
    } else if (dice == 1) {
        const auto pdf2Res = _pdf2->GenerateRayDirection();
        return pdf2Res ? pdf2Res : _pdfFallback->GenerateRayDirection();
    } else {
        return _pdfFallback->GenerateRayDirection();
    }
}

#endif  //MONTECARLORAYTRACER_MIX3PDF_HPP
