//
// Created by zhang on 12/13/19.
//

#ifndef MONTECARLORAYTRACER_PHOTONMAPPINGPDF_HPP
#define MONTECARLORAYTRACER_PHOTONMAPPINGPDF_HPP

#include "../rendering/PhotonMapping.hpp"
#include "CosinePdf.hpp"
#include "Pdf.hpp"

class PhotonMappingPDF: public PDF {
    public:
    PhotonMappingPDF(const PhotonMap& photonMap, const HitRecord& hitRecord)
        : _photonMap { photonMap }, _hitRecord { hitRecord } {
        const auto location    = hitRecord.GetLocation();
        const auto queryResult = photonMap.Query(location, QueryNum);
        const auto normal      = hitRecord.GetNorm();

        if (queryResult.empty() || queryResult[0].second > MaxDSAllowed) {
            _direction = normal;
        } else {
            glm::vec3 sum   = { 0.0f, 0.0f, 0.0f };
            auto      count = 0;
            for (const auto& entry : queryResult) {
                if (entry.second > MaxDSAllowed) break;
                sum += entry.first;
                count++;
            }
            _direction = glm::normalize(sum / (static_cast<float>(count)));
        }
    }

    virtual float GetPDFValue(const Direction& direction) const override {
        const auto normalizedDirection = glm::normalize(direction);
        if (glm::dot(normalizedDirection, _direction) < Threshold) {
            return 0;
        } else {
            return PDFValue;
        }
    }

    virtual Direction GenerateRayDirection() const override {
        const auto bases         = OrthoNormalBases::BuildFromW(_direction);
        const auto point         = RandomPointOnUnitDisk();
        const auto x             = point.x * Alpha;
        const auto y             = point.y * Alpha;
        const auto directionSend = bases.GetLocalLocation(x, y, 1);
        return directionSend;
    }

    private:
    const PhotonMap&       _photonMap;
    const HitRecord&       _hitRecord;
    Direction              _direction;
    static constexpr float Alpha        = Pi / 12;
    static constexpr float PDFValue     = Pi * Alpha * Alpha;
    static constexpr float Threshold    = 0.965926;
    static constexpr int   QueryNum     = 6;
    static constexpr float MaxDSAllowed = 16;
};

#endif  //MONTECARLORAYTRACER_PHOTONMAPPINGPDF_HPP
