//
// Created by zhang on 10/24/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_TRIANGLE_HPP
#define MONTE_CARLO_RAY_TRACER_TRIANGLE_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/transform.hpp>

#include "../bounded-volumn-hierarchy/BvhLeaf.hpp"
#include "../common/Utils.hpp"
#include "Hitable.hpp"

class Triangle: public Hitable {
    public:
    Triangle(
        const Location&                  vertex1,
        const Direction&                 norm1,
        const UVCoordinate&              uv1,
        const Location&                  vertex2,
        const Direction&                 norm2,
        const UVCoordinate&              uv2,
        const Location&                  vertex3,
        const Direction&                 norm3,
        const UVCoordinate&              uv3,
        const std::shared_ptr<Material>& material)
        : _vertex1 { vertex1 },
          _norm1 { norm1 },
          _uv1 { uv1 },
          _vertex2 { vertex2 },
          _norm2 { norm2 },
          _uv2 { uv2 },
          _vertex3 { vertex3 },
          _norm3 { norm3 },
          _uv3 { uv3 },
          _material { material },
          _edge12 { vertex2 - vertex1 },
          _edge13 { vertex3 - vertex1 } {}

    [[nodiscard]] std::optional<HitRecord> HitBy(const Ray& ray, float tMin, float tMax) const override {
        const auto pVec    = glm::cross(ray.GetDirection(), _edge13);
        const auto det     = glm::dot(_edge12, pVec);
        const auto epsilon = Epsilon;
        if (det > -epsilon && det < epsilon) return {};
        const auto invDet = 1 / det;
        const auto tVec   = ray.GetOrigin() - _vertex1;
        const auto u      = invDet * glm::dot(tVec, pVec);
        if (u < 0 || u > 1) return {};
        const auto qVec = glm::cross(tVec, _edge12);
        const auto v    = invDet * glm::dot(ray.GetDirection(), qVec);
        if (v < 0 || u + v > 1) return {};
        const auto t = invDet * glm::dot(_edge13, qVec);
        if (t > tMin && t < tMax) {
            const auto location = ray.GetOrigin() + t * ray.GetDirection();
            const auto norm     = (1 - u - v) * _norm1 + u * _norm2 + v * _norm3;
            const auto uv       = (1 - u - v) * _uv1 + u * _uv2 + v * _uv3;
            return { { location, norm, uv, t, _material } };
        } else {
            return {};
        }
    }

    [[nodiscard]] AlignedBox GetAlignedBox() const override {
        auto xMin = std::min(_vertex1.x, std::min(_vertex2.x, _vertex3.x));
        auto xMax = std::max(_vertex1.x, std::max(_vertex2.x, _vertex3.x));
        auto yMin = std::min(_vertex1.y, std::min(_vertex2.y, _vertex3.y));
        auto yMax = std::max(_vertex1.y, std::max(_vertex2.y, _vertex3.y));
        auto zMin = std::min(_vertex1.z, std::min(_vertex2.z, _vertex3.z));
        auto zMax = std::max(_vertex1.z, std::max(_vertex2.z, _vertex3.z));

        // in case of equal
        auto adjust = [=](auto& val1, auto& val2) {
            if (val1 == val2) {
                val1 -= Epsilon;
                val2 += Epsilon;
            }
        };
        adjust(xMin, xMax);
        adjust(yMin, yMax);
        adjust(zMin, zMax);

        return AlignedBox(xMin, xMax, yMin, yMax, zMin, zMax);
    }

    [[nodiscard]] std::unique_ptr<BVH> BuildBVH() const override {
        const auto alignedBox = GetAlignedBox();
        return std::make_unique<BVHLeaf>(alignedBox, shared_from_this());
    }

    void ApplyTranslation(float dx, float dy, float dz) {
        Offset offset(dx, dy, dz);
        _vertex1 += offset;
        _vertex2 += offset;
        _vertex3 += offset;
    }

    void ApplyRotation(float degrees, float x, float y, float z) {
        Direction direction(x, y, z);
        direction            = glm::normalize(direction);
        const auto rotateMat = glm::mat3(glm::rotate(glm::radians(degrees), direction));
        _vertex1             = rotateMat * _vertex1;
        _vertex2             = rotateMat * _vertex2;
        _vertex3             = rotateMat * _vertex3;
        _norm1               = rotateMat * _norm1;
        _norm2               = rotateMat * _norm2;
        _norm3               = rotateMat * _norm3;
        _edge12              = rotateMat * _edge12;
        _edge13              = rotateMat * _edge13;
    }

    void ApplyScaling(float scale) {
        _vertex1 *= scale;
        _vertex2 *= scale;
        _vertex3 *= scale;
        _edge12 *= scale;
        _edge13 *= scale;
    }

    [[nodiscard]] std::pair<Direction, float> GetRandomDirectionWithPDF(const Location& origin) const override {
        auto x = RandomFloatBetween(Epsilon, 1 - Epsilon);
        auto y = RandomFloatBetween(Epsilon, 1 - Epsilon);
        if (x + y > 1) {
            x = 1 - Epsilon - x;
            y = 1 - Epsilon - y;
        }
        const auto point     = x * _edge12 + y * _edge13 + _vertex1;
        const auto direction = point - origin;

        const auto maybeHitRecord = HitBy(Ray { origin, direction, 0 }, Epsilon, PosInfinity);
        if (maybeHitRecord) {
            const auto& hitRecord       = maybeHitRecord.value();
            const auto  area            = glm::length(glm::cross(_edge12, _edge13)) / 2;
            const auto  distanceSquared = hitRecord.GetT() * hitRecord.GetT() * glm::length2(direction);
            const auto  cosine          = std::abs(glm::dot(direction, hitRecord.GetNorm()) / glm::length(direction));
            return { direction, distanceSquared / (cosine * area) };
        } else {
            return { direction, 0 };
        }
    }

    private:
    Location           _vertex1;
    Direction          _norm1;
    const UVCoordinate _uv1;
    Location           _vertex2;
    Direction          _norm2;
    const UVCoordinate _uv2;
    Location           _vertex3;
    Direction          _norm3;
    const UVCoordinate _uv3;

    const std::shared_ptr<Material> _material;

    Offset _edge12;
    Offset _edge13;
};

#endif  // MONTE_CARLO_RAY_TRACER_SPHERE_HPP
