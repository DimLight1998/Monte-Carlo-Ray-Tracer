//
// Created by zhang on 12/13/19.
//

#ifndef MONTECARLORAYTRACER_PHOTONMAPPING_HPP
#define MONTECARLORAYTRACER_PHOTONMAPPING_HPP

#include <exception>
#include <memory>
#include <vector>

#include <nanoflann/nanoflann.hpp>

#include "../bounded-volumn-hierarchy/Bvh.hpp"
#include "../common/Typing.hpp"
#include "../common/Ray.hpp"

struct PhotonCloud {
    struct PhotonPoint {
        PhotonPoint(float x, float y, float z, const Direction& toLightSource)
            : X { x }, Y { y }, Z { z }, ToLightSource(toLightSource) {}
        float     X;
        float     Y;
        float     Z;
        Direction ToLightSource;
    };

    std::vector<PhotonPoint> PhotonPoints;

    [[nodiscard]] inline size_t kdtree_get_point_count() const {
        return PhotonPoints.size();
    }

    [[nodiscard]] inline float kdtree_get_pt(const size_t index, const size_t dim) const {
        if (dim == 0) return PhotonPoints[index].X;
        if (dim == 1) return PhotonPoints[index].Y;
        if (dim == 2) return PhotonPoints[index].Z;
        throw std::runtime_error("strange dim");
    }

    template<class BBOX>
    bool kdtree_get_bbox(BBOX&) const {
        return false;
    }
};

class PhotonMap {
    public:
    void Add(const Location& location, const Direction& toLightSource) {
        _photonCloud.PhotonPoints.emplace_back(location.x, location.y, location.z, toLightSource);
    }

    void FreezeThenBuildTree() {
        _kdTree = std::make_unique<KDTreeType>(3, _photonCloud, nanoflann::KDTreeSingleIndexAdaptorParams(10));
        _kdTree->buildIndex();
    }

    // direction and distance squared
    std::vector<std::pair<Direction, float>> Query(const Location& location, int numNearestNeighbors) const {
        float                                    query[] = { location.x, location.y, location.z };
        std::vector<std::pair<Direction, float>> ret;
        size_t                                   indices[numNearestNeighbors];
        float                                    distancesSquared[numNearestNeighbors];
        nanoflann::KNNResultSet<float>           resultSet(numNearestNeighbors);
        resultSet.init(indices, distancesSquared);
        _kdTree->findNeighbors(resultSet, query, nanoflann::SearchParams(10));
        for (auto i = 0; i < resultSet.size(); i++) {
            const auto index = indices[i];
            ret.emplace_back(_photonCloud.PhotonPoints[index].ToLightSource, distancesSquared[i]);
        }
        return ret;
    }

    [[nodiscard]] int GetCount() const {
        return _photonCloud.PhotonPoints.size();
    }

    [[nodiscard]] bool IsEmpty() const {
        return _photonCloud.PhotonPoints.empty();
    }

    private:
    using KDTreeType =
        nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<float, PhotonCloud>, PhotonCloud, 3>;

    PhotonCloud                 _photonCloud;
    std::unique_ptr<KDTreeType> _kdTree;
};

Ray GetRandomRayFromPMHitables(const std::vector<std::shared_ptr<const Hitable>>& pmImportantHitables) {
    const auto size  = static_cast<int>(pmImportantHitables.size());
    const auto index = RandomIntegerBetween(0, size - 1);
    return pmImportantHitables[index]->GenerateRandomRayForPM();
}

void AppendPhotonMappingData(
    const std::unique_ptr<BVH>&                  bvh,
    const Ray&                                   ray,
    int                                          maxDepth,
    std::vector<std::pair<Location, Direction>>& dataBuffer) {
    if (maxDepth == 0) return;
    const auto hitResult = bvh->HitBy(ray, Epsilon, PosInfinity);
    if (!hitResult) return;
    const auto& hitRecord = hitResult.value();
    dataBuffer.emplace_back(hitRecord.GetLocation(), -glm::normalize(ray.GetDirection()));
    const auto scattered = hitRecord.GetMaterial().Scattered(ray, hitRecord);
    if (scattered && scattered.value().GetMaybeSpecularRay()) {
        const auto& specularRay = scattered.value().GetMaybeSpecularRay().value();
        AppendPhotonMappingData(bvh, specularRay, maxDepth - 1, dataBuffer);
    }
}

#endif  //MONTECARLORAYTRACER_PHOTONMAPPING_HPP
