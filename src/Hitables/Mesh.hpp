//
// Created by zhang on 10/24/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_MESH_HPP
#define MONTE_CARLO_RAY_TRACER_MESH_HPP

#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include <string>
#include <vector>

#include <obj_loader/tiny_obj_loader.h>

#include "Hitable.hpp"
#include "Triangle.hpp"

class Mesh : public Hitable {
  public:
    explicit Mesh(const std::string &objPath, const std::shared_ptr<Material> &material) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        // discard materials
        std::vector<tinyobj::material_t> materials;
        std::string err;
        const auto ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objPath.c_str());
        if (!ret)
            throw std::runtime_error("object load failed: " + err);
        std::vector<Location> vertices;
        std::vector<Direction> normals;
        std::vector<UVCoordinate> uvs;
        for (auto &shape : shapes) {
            auto indexOffset = 0ull;
            for (auto f = 0ull; f < shape.mesh.num_face_vertices.size(); f++) {
                vertices.clear();
                normals.clear();
                uvs.clear();

                const auto fv = shape.mesh.num_face_vertices[f];
                const auto hasNormals = !attrib.normals.empty();
                const auto hasUVs = !attrib.texcoords.empty();
                for (auto v = 0; v < fv; v++) {
                    const auto idx = shape.mesh.indices[indexOffset + v];
                    const auto vx = attrib.vertices[3 * idx.vertex_index + 0];
                    const auto vy = attrib.vertices[3 * idx.vertex_index + 1];
                    const auto vz = attrib.vertices[3 * idx.vertex_index + 2];
                    vertices.emplace_back(Location{vx, vy, vz});

                    if (hasNormals) {
                        const auto nx = attrib.normals[3 * idx.normal_index + 0];
                        const auto ny = attrib.normals[3 * idx.normal_index + 1];
                        const auto nz = attrib.normals[3 * idx.normal_index + 2];
                        normals.emplace_back(nx, ny, nz);
                    }

                    if (hasUVs) {
                        const auto tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                        const auto ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                        uvs.emplace_back(UVCoordinate{tx, ty});
                    } else {
                        uvs.emplace_back(UVCoordinate{0, 0});
                    }
                }
                indexOffset += fv;

                if (hasNormals) {
                    _triangles.emplace_back(Triangle(vertices[0], normals[0], uvs[0], vertices[1], normals[1], uvs[1],
                                                     vertices[2], normals[2], uvs[2], material));
                } else {
                    const auto edge1 = vertices[0] - vertices[1];
                    const auto edge2 = vertices[0] - vertices[2];
                    const auto norm1 = glm::normalize(glm::cross(edge1, edge2));
                    const auto norm2 = norm1;
                    _triangles.emplace_back(Triangle(vertices[0], norm1, uvs[0], vertices[1], norm1, uvs[1],
                                                     vertices[2], norm1, uvs[2], material));
                    _triangles.emplace_back(Triangle(vertices[0], norm2, uvs[0], vertices[1], norm2, uvs[1],
                                                     vertices[2], norm2, uvs[2], material));
                }
            }
        }

        const auto max = std::numeric_limits<float>::max();
        const auto min = std::numeric_limits<float>::min();
        AlignedBox alignedBox;
        for (const auto &triangle : _triangles) {
            alignedBox = alignedBox | triangle.GetAlignedBox();
        }
        _alignedBoxPrecomputed = alignedBox;
    }

    [[nodiscard]] std::optional<HitRecord> HitBy(const Ray &ray, float tMin, float tMax) const override {
        // this function won't be called since it is a tree of BVH
        throw std::runtime_error("should not be called");
    }

    [[nodiscard]] AlignedBox GetAlignedBox() const override { return _alignedBoxPrecomputed; }

    [[nodiscard]] std::unique_ptr<BVH> BuildBVH() const override {
        std::vector<std::shared_ptr<const Hitable>> triangles;
        std::for_each(_triangles.begin(), _triangles.end(),
                      [&](const auto &triangle) { triangles.emplace_back(triangle.shared_from_this()); });
        return BVH::BuildBVH(triangles);
    }

    Mesh &ApplyTranslation(float dx, float dy, float dz) {
        std::for_each(_triangles.begin(), _triangles.end(),
                      [=](auto &triangle) { triangle.ApplyTranslation(dx, dy, dz); });
        RebuildAlignedBox();
        return *this;
    }

    Mesh &ApplyRotation(float degrees, float x, float y, float z) {
        std::for_each(_triangles.begin(), _triangles.end(),
                      [=](auto &triangle) { triangle.ApplyRotation(degrees, x, y, z); });
        RebuildAlignedBox();
        return *this;
    }

    Mesh &ApplyScaling(float scale) {
        std::for_each(_triangles.begin(), _triangles.end(), [=](auto &triangle) { triangle.ApplyScaling(scale); });
        RebuildAlignedBox();
        return *this;
    }

  protected:
    std::vector<Triangle> _triangles;
    AlignedBox _alignedBoxPrecomputed;

    Mesh() = default;

    void RebuildAlignedBox() {
        AlignedBox alignedBox;
        for (const auto &triangle : _triangles) {
            alignedBox = alignedBox | triangle.GetAlignedBox();
        }
        _alignedBoxPrecomputed = alignedBox;
    }
};

#endif // MONTE_CARLO_RAY_TRACER_MESH_HPP
