//
// Created by zhang on 10/23/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_HITRECORD_HPP
#define MONTE_CARLO_RAY_TRACER_HITRECORD_HPP

#include <glm/glm.hpp>
#include <memory>
#include <utility>

#include "../Materials/Material.hpp"

class Material;

class HitRecord {
    public:
    HitRecord(
        const glm::vec3&                 location,
        const glm::vec3&                 norm,
        const UVCoordinate&              uv,
        float                            t,
        const std::shared_ptr<Material>& material)
        : _location(location), _norm(norm), _uv(uv), _t(t), _material(material) {}

    const glm::vec3& GetLocation() const {
        return _location;
    }

    const glm::vec3& GetNorm() const {
        return _norm;
    }

    const UVCoordinate& GetUv() const {
        return _uv;
    }

    float GetT() const {
        return _t;
    }

    const Material& GetMaterial() const {
        return *_material;
    }

    private:
    glm::vec3                       _location;
    glm::vec3                       _norm;
    UVCoordinate                    _uv;
    float                           _t;
    const std::shared_ptr<Material> _material;
};

#endif  // MONTE_CARLO_RAY_TRACER_HITRECORD_HPP
