//
// Created by zhang on 12/15/19.
//

#ifndef MONTECARLORAYTRACER_BEZIER_HPP
#define MONTECARLORAYTRACER_BEZIER_HPP

#include <vector>

#include <glm/glm.hpp>

#include "../Triangle.hpp"
#include "BezierPatch.hpp"

class Bezier {
    public:
    explicit Bezier(const std::vector<std::vector<std::vector<glm::vec3>>>& patchesData) {
        for (const auto& patchData : patchesData) {
            _patches.emplace_back(patchData);
        }
    }

    void ApplyAdaptiveSubdivision(float error) {
        for (auto& patch : _patches) {
            patch.ApplyAdaptiveSubdivision(error);
        }
    }

    [[nodiscard]] std::vector<Triangle> ToTriangles(const std::shared_ptr<Material>& material) const {
        std::vector<Triangle> ret;
        const auto            uv0 = UVCoordinate(0, 0);

        for (const auto& patch : _patches) {
            const auto& points  = patch.GetAdaptivePoints();
            const auto& normals = patch.GetAdaptiveNormals();
            for (auto i = 0; i < points.size(); i++) {
                ret.emplace_back(
                    points[i][0],
                    glm::normalize(normals[i][0]),
                    uv0,
                    points[i][1],
                    glm::normalize(normals[i][1]),
                    uv0,
                    points[i][2],
                    glm::normalize(normals[i][2]),
                    uv0,
                    material);
            }
        }

        return ret;
    }

    private:
    std::vector<BezierPatch> _patches = std::vector<BezierPatch>();
};

#endif  //MONTECARLORAYTRACER_BEZIER_HPP
