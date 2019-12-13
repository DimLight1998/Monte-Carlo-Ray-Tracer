//
// Created by zhang on 11/22/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_MULTIPLEHITABLESPDF_HPP
#define MONTE_CARLO_RAY_TRACER_MULTIPLEHITABLESPDF_HPP

#include <algorithm>
#include <memory>

#include "../hitables/Hitable.hpp"
#include "Pdf.hpp"

class MultipleHitablesPDF: public PDF {
    public:
    MultipleHitablesPDF(const std::vector<std::shared_ptr<const Hitable>>& hitables, const Location& origin);

    [[nodiscard]] float                    GetPDFValue(const Direction& direction) const override;
    [[nodiscard]] std::optional<Direction> GenerateRayDirection() const override;

    private:
    std::vector<std::shared_ptr<const Hitable>> _hitables;
    Location                                    _origin;
};

[[nodiscard]] std::optional<Direction> MultipleHitablesPDF::GenerateRayDirection() const {
    if (_hitables.empty()) return {};
    const auto index = RandomIntegerBetween(0, _hitables.size() - 1);
    return { _hitables[index]->GetRandomRayDirection(_origin) };
}

[[nodiscard]] float MultipleHitablesPDF::GetPDFValue(const Direction& direction) const {
    if (_hitables.empty()) return 0;
    auto sum = 0.0f;
    for (const auto& hitable : _hitables) {
        sum += hitable->GetRayPDF(_origin, direction);
    }
    sum /= _hitables.size();
    return sum;
}

MultipleHitablesPDF::MultipleHitablesPDF(
    const std::vector<std::shared_ptr<const Hitable>>& hitables,
    const Location&                                    origin) {
    _origin   = origin;
    _hitables = hitables;
}

#endif  //MONTE_CARLO_RAY_TRACER_MULTIPLEHITABLESPDF_HPP
