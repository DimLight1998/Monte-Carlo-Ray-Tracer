//
// Created by zhang on 12/15/19.
//

#ifndef MONTECARLORAYTRACER_BEZIERPATCH_HPP
#define MONTECARLORAYTRACER_BEZIERPATCH_HPP

#include <vector>

#include "../../common/Utils.hpp"
#include <glm/glm.hpp>
#include <tuple>

class BezierPatch {
    public:
    explicit BezierPatch(const std::vector<std::vector<glm::vec3>>& patchData) {
        _controlPoints = patchData;
    }

    void ApplyAdaptiveSubdivision(float error) {
        const auto stepSize = 1.0f / _controlPoints.size();

        for (auto i = 0; i < _controlPoints.size(); i++) {
            for (auto j = 0; j < _controlPoints[i].size(); j++) {
                const auto uvA = glm::vec2 { i * stepSize, j * stepSize };
                const auto uvB = glm::vec2 { (i + 1.0f) * stepSize, j * stepSize };
                const auto uvC = glm::vec2 { i * stepSize, (j + 1.0f) * stepSize };
                const auto uvD = glm::vec2 { (i + 1.0f) * stepSize, (j + 1.0f) * stepSize };

                const auto vtA = CalculateUVPoints(i * stepSize, j * stepSize);
                const auto vtB = CalculateUVPoints((i + 1.0f) * stepSize, j * stepSize);
                const auto vtC = CalculateUVPoints(i * stepSize, (j + 1.0f) * stepSize);
                const auto vtD = CalculateUVPoints((i + 1.0f) * stepSize, (j + 1.0f) * stepSize);

                const std::vector<glm::vec3> verticesACB = { vtA, vtC, vtB };
                const std::vector<glm::vec2> uvACB       = { uvA, uvC, uvB };
                SubdivideTriangle(verticesACB, uvACB, error);

                const std::vector<glm::vec3> verticesBCD = { vtB, vtC, vtD };
                const std::vector<glm::vec2> uvBCD       = { uvB, uvC, uvD };
                SubdivideTriangle(verticesBCD, uvBCD, error);
            }
        }
    }

    [[nodiscard]] const std::vector<std::vector<glm::vec3>>& GetAdaptiveNormals() const {
        return _adaptiveNormals;
    }

    [[nodiscard]] const std::vector<std::vector<glm::vec3>>& GetAdaptivePoints() const {
        return _adaptivePoints;
    }

    private:
    std::vector<std::vector<glm::vec3>> _controlPoints   = std::vector<std::vector<glm::vec3>>();
    std::vector<std::vector<glm::vec3>> _adaptiveNormals = std::vector<std::vector<glm::vec3>>();
    std::vector<std::vector<glm::vec3>> _adaptivePoints  = std::vector<std::vector<glm::vec3>>();

    glm::vec3 CalculateUVPoints(float u, float v) {
        const auto uVec = glm::vec4 { std::pow(u, 3), std::pow(u, 2), u, 1 };
        const auto vVec = glm::vec4 { std::pow(v, 3), std::pow(v, 2), v, 1 };
        const auto mat  = glm::mat4x4 { { -1, 3, -3, 1 }, { 3, -5, 3, 0 }, { -3, 3, 0, 0 }, { 1, 0, 0, 0 } };
        const auto matT = glm::transpose(mat);

        const auto controlXs = GetControlPointsOfDimension(0);
        const auto controlYs = GetControlPointsOfDimension(1);
        const auto controlZs = GetControlPointsOfDimension(2);
        const auto x         = glm::dot(uVec * mat * controlXs * matT, vVec);
        const auto y         = glm::dot(uVec * mat * controlYs * matT, vVec);
        const auto z         = glm::dot(uVec * mat * controlZs * matT, vVec);
        return { x, y, z };
    }

    glm::vec3 CalculateUVNormal(float u, float v, float delta) {
        const auto p  = CalculateUVPoints(u, v);
        const auto pU = CalculateUVPoints(u + delta, v);
        const auto pV = CalculateUVPoints(u, v + delta);
        const auto n  = glm::cross(pU - p, pV - p);
        return (glm::length(n) <= Epsilon) ? CalculateUVNormal(u + delta, v + delta, Epsilon) : n;
    }

    glm::mat4 GetControlPointsOfDimension(int dimension) {
        const auto& p = _controlPoints;
        glm::mat4   ret;
        switch (dimension) {
        case 0:
            for (auto i = 0; i < 4; i++) {
                for (auto j = 0; j < 4; j++) {
                    ret[i][j] = _controlPoints[i][j].x;
                }
            }
            return ret;
        case 1:
            for (auto i = 0; i < 4; i++) {
                for (auto j = 0; j < 4; j++) {
                    ret[i][j] = _controlPoints[i][j].y;
                }
            }
            return ret;
        case 2:
            for (auto i = 0; i < 4; i++) {
                for (auto j = 0; j < 4; j++) {
                    ret[i][j] = _controlPoints[i][j].z;
                }
            }
            return ret;
        default:
            throw std::runtime_error("invalid dimension");
        }
    }

    void SubdivideTriangle(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, float error) {
        const auto& vt0 = vertices[0];
        const auto& vt1 = vertices[1];
        const auto& vt2 = vertices[2];
        const auto& uv0 = uvs[0];
        const auto& uv1 = uvs[1];
        const auto& uv2 = uvs[2];

        const auto check = [=](const glm::vec3& vtA, const glm::vec3& vtB, const glm::vec2& uvA, const glm::vec2& uvB) {
            const auto uvMid = (uvA + uvB) / 2.0f;
            const auto ref   = CalculateUVPoints(uvMid.x, uvMid.y);
            const auto vtMid = (vtA + vtB) / 2.0f;
            return std::make_tuple(glm::distance(vtMid, ref) < error, ref, uvMid);
        };

        const auto  res01   = check(vt0, vt1, uv0, uv1);
        const auto  res12   = check(vt1, vt2, uv1, uv2);
        const auto  res20   = check(vt2, vt0, uv2, uv0);
        const auto  ok01    = std::get<0>(res01);
        const auto  ok12    = std::get<0>(res12);
        const auto  ok20    = std::get<0>(res20);
        const auto& ref01   = std::get<1>(res01);
        const auto& ref12   = std::get<1>(res12);
        const auto& ref20   = std::get<1>(res20);
        const auto& mid01   = std::get<2>(res01);
        const auto& mid12   = std::get<2>(res12);
        const auto& mid20   = std::get<2>(res20);
        const auto  okCount = (ok01 ? 1 : 0) + (ok12 ? 1 : 0) + (ok20 ? 1 : 0);

        std::vector<std::vector<glm::vec3>> triangleVertices;
        std::vector<std::vector<glm::vec2>> triangleUVs;

        if (okCount == 3) {
            std::vector<glm::vec3> normals = { CalculateUVNormal(vt0.x, vt0.y, Epsilon),
                                               CalculateUVNormal(vt1.x, vt1.y, Epsilon),
                                               CalculateUVNormal(vt2.x, vt2.y, Epsilon) };
            _adaptiveNormals.emplace_back(normals);
            _adaptivePoints.emplace_back(vertices);
        } else if (okCount == 2) {
            if (!ok01) {
                triangleVertices.assign({ { vt0, ref01, vt2 }, { vt2, ref01, vt1 } });
                triangleUVs.assign({ { uv0, mid01, uv2 }, { uv2, ref01, uv1 } });
            } else if (!ok12) {
                triangleVertices.assign({ { ref12, vt0, vt1 }, { vt0, ref12, vt2 } });
                triangleUVs.assign({ { mid12, uv0, uv1 }, { uv0, mid12, uv2 } });
            } else if (!ok20) {
                triangleVertices.assign({ { vt0, vt1, ref20 }, { vt1, vt2, ref20 } });
                triangleUVs.assign({ { uv0, uv1, mid20 }, { uv1, uv2, mid20 } });
            }
        } else if (okCount == 1) {
            if (!ok01 && !ok12) {
                triangleVertices.assign({ { vt1, ref12, ref01 }, { ref01, ref12, vt2 }, { vt0, ref01, vt2 } });
                triangleUVs.assign({ { uv1, mid12, mid01 }, { mid01, mid12, uv2 }, { uv0, mid01, uv2 } });
            } else if (!ok12 && !ok20) {
                triangleVertices.assign({ { ref20, ref12, vt2 }, { ref12, ref20, vt1 }, { vt1, ref20, vt0 } });
                triangleUVs.assign({ { mid20, mid12, uv2 }, { mid12, mid20, uv1 }, { uv1, mid20, uv0 } });
            } else if (!ok20 && !ok01) {
                triangleVertices.assign({ { vt0, ref01, ref20 }, { ref20, ref01, vt2 }, { vt2, ref01, vt1 } });
                triangleUVs.assign({ { uv0, mid01, mid20 }, { mid20, mid01, uv2 }, { uv2, mid01, uv1 } });
            }
        } else if (okCount == 0) {
            triangleVertices.assign(
                { { ref20, ref12, vt2 }, { ref12, ref01, vt1 }, { vt0, ref01, ref20 }, { ref12, ref20, ref01 } });
            triangleUVs.assign(
                { { mid20, mid12, uv2 }, { mid12, mid01, uv1 }, { uv0, mid01, mid20 }, { mid12, mid20, mid01 } });
        }

        for (auto i = 0; i < triangleVertices.size(); i++) {
            SubdivideTriangle(triangleVertices[i], triangleUVs[i], error);
        }
    }
};

#endif  //MONTECARLORAYTRACER_BEZIERPATCH_HPP
