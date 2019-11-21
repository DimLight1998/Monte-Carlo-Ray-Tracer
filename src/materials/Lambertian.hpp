//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_LAMBERTIAN_HPP
#define MONTE_CARLO_RAY_TRACER_LAMBERTIAN_HPP

#include <memory>

#include "../common/Utils.hpp"
#include "../pdf/CosinePdf.hpp"
#include "../pdf/Pdf.hpp"
#include "../textures/Texture.hpp"
#include "Material.hpp"

class Lambertian: public Material {
    public:
    explicit Lambertian(const std::shared_ptr<Texture>& texture): _texture { texture } {}

    [[nodiscard]] std::optional<ScatterRecord> Scattered(const Ray& ray, const HitRecord& hitRecord) const override {
        const auto attenuation = _texture->GetTextureColorAt(hitRecord.GetUv(), hitRecord.GetLocation());
        const auto pdfPtr      = std::shared_ptr<PDF>(new CosinePDF(hitRecord.GetNorm()));
        return { { attenuation, pdfPtr, {} } };
    }

    virtual float
    GetScatteringPDF(const Ray& rayIn, const HitRecord& hitRecord, const Ray& rayScattered) const override {
        const auto cosine = std::max(glm::dot(hitRecord.GetNorm(), glm::normalize(rayScattered.GetDirection())), 0.0f);
        return cosine / Pi;
    }

    private:
    const std::shared_ptr<Texture> _texture;
};

#endif  // MONTE_CARLO_RAY_TRACER_LAMBERTIAN_HPP
