//
// Created by zhang on 12/13/19.
//

#ifndef MONTECARLORAYTRACER_PHOTONMAPPINGPDF_HPP
#define MONTECARLORAYTRACER_PHOTONMAPPINGPDF_HPP

#include "Pdf.hpp"

#include "../rendering/PhotonMapping.hpp"

class PhotonMappingPDF: public PDF {
    public:
    PhotonMappingPDF(const PhotonMap& photonMap, const HitRecord& hitRecord)
        : _photonMap { photonMap }, _hitRecord { hitRecord } {
        const auto location    = hitRecord.GetLocation();
        const auto queryResult = photonMap.Query(location, QueryNum);
    }

    virtual float GetPDFValue(const Direction& direction) const override {
        return 0;  // todo
    }

    virtual std::optional<Direction> GenerateRayDirection() const override {
        if (_photonMap.IsEmpty()) return {};
        return { Direction() };  // todo
    }

    private:
    const PhotonMap&     _photonMap;
    const HitRecord&     _hitRecord;
    float                _alpha   = Pi / 12;
    static constexpr int QueryNum = 4;
};

#endif  //MONTECARLORAYTRACER_PHOTONMAPPINGPDF_HPP
