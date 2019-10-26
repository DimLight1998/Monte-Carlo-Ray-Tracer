//
// Created by zhang on 10/25/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_RECTANGLE_HPP
#define MONTE_CARLO_RAY_TRACER_RECTANGLE_HPP

#include "../Mesh.hpp"

class Rectangle : public Mesh {
  public:
    /**
     * @brief Get a rectangle from (0, 0, 0) to (width, height, 0), with normal pointing to (0, 0, 1).
     */
    Rectangle(float height, float width, const std::shared_ptr<Material> &material) {
        const auto topRight = Location(width, height, 0);
        const auto bottomRight = Location(width, 0, 0);
        const auto topLeft = Location(0, height, 0);
        const auto bottomLeft = Location(0, 0, 0);
        const auto front = Direction(0, 0, 1);
        const auto uv0 = UVCoordinate(0, 0);
        auto triangles =
            std::vector<Triangle>{{topRight, front, uv0, bottomRight, front, uv0, topLeft, front, uv0, material},
                                  {bottomLeft, front, uv0, bottomRight, front, uv0, topLeft, front, uv0, material}};
        _triangles = std::move(triangles);
        RebuildAlignedBox();
    }

    static Rectangle OfXY(float xMin, float xMax, float yMin, float yMax, float z, bool positiveNorm,
                          const std::shared_ptr<Material> &material) {
        const auto farFar = Location(xMax, yMax, z);
        const auto nearFar = Location(xMin, yMax, z);
        const auto farNear = Location(xMax, yMin, z);
        const auto nearNear = Location(xMin, yMin, z);
        const auto norm = positiveNorm ? Direction(0, 0, 1) : Direction(0, 0, -1);
        const auto uv0 = UVCoordinate(0, 0);
        auto triangles = std::vector<Triangle>{{farFar, norm, uv0, farNear, norm, uv0, nearFar, norm, uv0, material},
                                               {nearNear, norm, uv0, farNear, norm, uv0, nearFar, norm, uv0, material}};
        Rectangle ret;
        ret._triangles = std::move(triangles);
        ret.RebuildAlignedBox();
        return ret;
    }

    static Rectangle OfYZ(float yMin, float yMax, float zMin, float zMax, float x, bool positiveNorm,
                          const std::shared_ptr<Material> &material) {
        const auto farFar = Location(x, yMax, zMax);
        const auto nearFar = Location(x, yMin, zMax);
        const auto farNear = Location(x, yMax, zMin);
        const auto nearNear = Location(x, yMin, zMin);
        const auto norm = positiveNorm ? Direction(1, 0, 0) : Direction(-1, 0, 0);
        const auto uv0 = UVCoordinate(0, 0);
        auto triangles = std::vector<Triangle>{{farFar, norm, uv0, farNear, norm, uv0, nearFar, norm, uv0, material},
                                               {nearNear, norm, uv0, farNear, norm, uv0, nearFar, norm, uv0, material}};
        Rectangle ret;
        ret._triangles = std::move(triangles);
        ret.RebuildAlignedBox();
        return ret;
    }

    static Rectangle OfZX(float zMin, float zMax, float xMin, float xMax, float y, bool positiveNorm,
                          const std::shared_ptr<Material> &material) {
        const auto farFar = Location(xMax, y, zMax);
        const auto nearFar = Location(xMax, y, zMin);
        const auto farNear = Location(xMin, y, zMax);
        const auto nearNear = Location(xMin, y, zMin);
        const auto norm = positiveNorm ? Direction(0, 1, 0) : Direction(0, -1, 0);
        const auto uv0 = UVCoordinate(0, 0);
        auto triangles = std::vector<Triangle>{{farFar, norm, uv0, farNear, norm, uv0, nearFar, norm, uv0, material},
                                               {nearNear, norm, uv0, farNear, norm, uv0, nearFar, norm, uv0, material}};
        Rectangle ret;
        ret._triangles = std::move(triangles);
        ret.RebuildAlignedBox();
        return ret;
    }

  private:
    Rectangle() = default;
};

#endif // MONTE_CARLO_RAY_TRACER_RECTANGLE_HPP
