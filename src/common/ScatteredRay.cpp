//
// Created by zhang on 11/20/19.
//

#include "ScatteredRay.hpp"
ScatteredRay::ScatteredRay(const Attenuation& attenuation, const Ray& ray, float pdf)
    : _attenuation(attenuation), _ray(ray), _pdf(pdf) {}

const Attenuation& ScatteredRay::GetAttenuation() const {
    return _attenuation;
}

const Ray& ScatteredRay::GetRay() const {
    return _ray;
}

float ScatteredRay::GetPdf() const {
    return _pdf;
}
