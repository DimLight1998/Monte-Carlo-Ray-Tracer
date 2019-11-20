//
// Created by zhang on 11/20/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_SCATTEREDRAY_HPP
#define MONTE_CARLO_RAY_TRACER_SCATTEREDRAY_HPP

#include "Ray.hpp"
#include "Utils.hpp"

class ScatteredRay {
    public:
    ScatteredRay(const Attenuation& attenuation, const Ray& ray, float pdf);

    const Attenuation& GetAttenuation() const;
    const Ray&         GetRay() const;
    float              GetPdf() const;

    private:
    Attenuation _attenuation;
    Ray         _ray;
    float       _pdf;
};

#endif  //MONTE_CARLO_RAY_TRACER_SCATTEREDRAY_HPP
