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
        if (queryResult.empty() || queryResult[0].second > MaxDSAllowed) {
            _shouldReject = true;
            return;
        } else {
            glm::vec3 sum   = { 0.0f, 0.0f, 0.0f };
            auto      count = 0;
            for (const auto& entry : queryResult) {
                if (entry.second > MaxDSAllowed) break;
                sum += entry.first;
                count++;
            }
            _direction              = glm::normalize(sum / (static_cast<float>(count)));
            const auto normal       = hitRecord.GetNorm();
            const auto cosAlphaComp = glm::dot(_direction, normal);
            if (cosAlphaComp <= 0) {
                _shouldReject = true;
                return;
            }
            const auto alphaComp = std::acos(cosAlphaComp);
            const auto alpha     = Pi / 2 - alphaComp;
            _alpha               = std::min(alpha, _alpha);
            _pdfValue            = 1 / (Pi * _alpha * _alpha);
        }
    }

    virtual float GetPDFValue(const Direction& direction) const override {
        const auto normalizedDirection = glm::normalize(direction);
        const auto threshold           = std::cos(_alpha);
        if (glm::dot(normalizedDirection, _direction) < threshold) {
            return 0;
        } else {
            return _pdfValue;
        }
    }

    virtual std::optional<Direction> GenerateRayDirection() const override {
        if (_photonMap.IsEmpty() || _shouldReject) return {};
        const auto bases         = OrthoNormalBases::BuildFromW(_direction);
        const auto point         = RandomPointOnUnitDisk();
        const auto x             = point.x * _alpha;
        const auto y             = point.y * _alpha;
        const auto directionSend = bases.GetLocalLocation(x, y, 1);
        return { directionSend };
    }

    private:
    const PhotonMap&       _photonMap;
    const HitRecord&       _hitRecord;
    Direction              _direction;
    float                  _alpha        = Pi / 12;
    float                  _pdfValue     = 0;
    bool                   _shouldReject = false;
    static constexpr int   QueryNum      = 4;
    static constexpr float MaxDSAllowed  = 1;
};

#endif  //MONTECARLORAYTRACER_PHOTONMAPPINGPDF_HPP
