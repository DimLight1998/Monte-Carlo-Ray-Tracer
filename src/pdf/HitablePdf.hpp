//
// Created by zhang on 11/20/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_HITABLEPDF_HPP
#define MONTE_CARLO_RAY_TRACER_HITABLEPDF_HPP

#include "Pdf.hpp"

#include "../hitables/Hitable.hpp"

class HitablePDF: public PDF {
    public:
    HitablePDF(std::shared_ptr<const Hitable> hitable, const Location& origin);

    [[nodiscard]] float     GetPDFValue(const Direction& direction) const override;
    [[nodiscard]] Direction GenerateRayDirection() const override;

    private:
    std::shared_ptr<const Hitable> _hitable;
    Location                       _origin;
};

HitablePDF::HitablePDF(std::shared_ptr<const Hitable> hitable, const Location& origin)
    : _hitable(std::move(hitable)), _origin(origin) {}

[[nodiscard]] float HitablePDF::GetPDFValue(const Direction& direction) const {
    return _hitable->GetRayPDF(_origin, direction);
}

[[nodiscard]] Direction HitablePDF::GenerateRayDirection() const {
    return _hitable->GetRandomRayDirection(_origin);
}

#endif  //MONTE_CARLO_RAY_TRACER_HITABLEPDF_HPP
