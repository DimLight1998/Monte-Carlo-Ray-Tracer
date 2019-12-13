//
// Created by zhang on 12/13/19.
//

#ifndef MONTECARLORAYTRACER_PHOTONMAPPINGPDF_HPP
#define MONTECARLORAYTRACER_PHOTONMAPPINGPDF_HPP

#include "../rendering/PhotonMapping.hpp"
#include "Pdf.hpp"

class PhotonMappingPDF: public PDF {
    public:
    explicit PhotonMappingPDF(const PhotonMap& photonMap, const HitRecord& hitRecord)
        : _photonMap { photonMap }, _hitRecord { hitRecord } {
        const auto location = hitRecord.GetLocation();
        const auto queryResult = photonMap.Query(location, QueryNum);
        // todo
    }

    virtual float GetPDFValue(const Direction& direction) const override {
        return 0;
    }

    virtual Direction GenerateRayDirection() const override {
        return Direction();
    }

    private:
    const PhotonMap& _photonMap;
    const HitRecord& _hitRecord;
    static constexpr int QueryNum = 4;
};

#endif  //MONTECARLORAYTRACER_PHOTONMAPPINGPDF_HPP
