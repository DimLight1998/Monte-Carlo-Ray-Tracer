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
    Rectangle(float height, float width, const Material &material) {
        const auto topRight = Location(width, height, 0);
        const auto bottomRight = Location(width, 0, 0);
        const auto topLeft = Location(0, height, 0);
        const auto bottomLeft = Location(0, 0, 0);
        const auto front = Direction(0, 0, 1);
        const auto uv0 = UVCoordinate(0, 0);
        auto triangles = std::vector<Triangle>{
            {topRight,   front, uv0, bottomRight, front, uv0, topLeft, front, uv0, material},
            {bottomLeft, front, uv0, bottomRight, front, uv0, topLeft, front, uv0, material}
        };
        _triangles = std::move(triangles);
        RebuildAlignedBox();
    }
};


#endif //MONTE_CARLO_RAY_TRACER_RECTANGLE_HPP
