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
        : _pdf1 { std::move(pdf1) }, _pdf2 { std::move(pdf2) }, _pdf3 { std::move(pdf3) } {}

    [[nodiscard]] float GetPDFValue(const Direction& direction) const override;

    [[nodiscard]] Direction GenerateRayDirection() const override;

    private:
    std::shared_ptr<PDF> _pdf1;
    std::shared_ptr<PDF> _pdf2;
    std::shared_ptr<PDF> _pdf3;
};

float Mix3PDF::GetPDFValue(const Direction& direction) const {
    const auto sum = _pdf1->GetPDFValue(direction) + _pdf2->GetPDFValue(direction) + _pdf3->GetPDFValue(direction);
    return sum / 3;
}

Direction Mix3PDF::GenerateRayDirection() const {
    const auto dice = RandomIntegerBetween(0, 2);
    if (dice == 0) {
        return _pdf1->GenerateRayDirection();
    } else if (dice == 1) {
        return _pdf2->GenerateRayDirection();
    } else {
        return _pdf3->GenerateRayDirection();
    }
}

#endif  //MONTECARLORAYTRACER_MIX3PDF_HPP
