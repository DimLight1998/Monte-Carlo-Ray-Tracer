//
// Created by zhang on 11/20/19.
//

#include "ScatterRecord.hpp"

ScatterRecord::ScatterRecord(
    const Attenuation&   attenuation,
    std::shared_ptr<PDF> pdf,
    std::optional<Ray>   maybeSpecularRay)
    : _attenuation { attenuation }, _pdf { std::move(pdf) }, _maybeSpecularRay { maybeSpecularRay } {}

const Attenuation& ScatterRecord::GetAttenuation() const {
    return _attenuation;
}

const std::shared_ptr<PDF>& ScatterRecord::GetPDF() const {
    return _pdf;
}

const std::optional<Ray>& ScatterRecord::GetMaybeSpecularRay() const {
    return _maybeSpecularRay;
}
