//
// Created by zhang on 11/20/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_SCATTERRECORD_HPP
#define MONTE_CARLO_RAY_TRACER_SCATTERRECORD_HPP

#include <memory>
#include <optional>

#include "../pdf/Pdf.hpp"
#include "Ray.hpp"
#include "Utils.hpp"

class ScatterRecord {
    public:
    ScatterRecord(const Attenuation& attenuation, std::shared_ptr<PDF> pdf, std::optional<Ray> maybeSpecularRay);

    const Attenuation&          GetAttenuation() const;
    const std::shared_ptr<PDF>& GetPDF() const;
    const std::optional<Ray>&   GetMaybeSpecularRay() const;

    private:
    Attenuation          _attenuation;
    std::shared_ptr<PDF> _pdf;
    std::optional<Ray>   _maybeSpecularRay;
};

#endif  //MONTE_CARLO_RAY_TRACER_SCATTERRECORD_HPP
