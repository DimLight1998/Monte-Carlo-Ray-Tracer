//
// Created by zhang on 10/25/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_CUBE_HPP
#define MONTE_CARLO_RAY_TRACER_CUBE_HPP

#include "../Mesh.hpp"

class Cube : public Mesh {
  public:
    explicit Cube(const std::shared_ptr<Material> &material) {
        const auto rightTopFront = Location(1, 1, 1);
        const auto rightTopBack = Location(1, 1, -1);
        const auto rightBottomFront = Location(1, -1, 1);
        const auto rightBottomBack = Location(1, -1, -1);
        const auto leftTopFront = Location(-1, 1, 1);
        const auto leftTopBack = Location(-1, 1, -1);
        const auto leftBottomFront = Location(-1, -1, 1);
        const auto leftBottomBack = Location(-1, -1, -1);
        const auto right = Direction(1, 0, 0);
        const auto left = Direction(-1, 0, 0);
        const auto top = Direction(0, 1, 0);
        const auto bottom = Direction(0, -1, 0);
        const auto front = Direction(0, 0, 1);
        const auto back = Direction(0, 0, -1);
        const auto uv0 = UVCoordinate(0, 0);
        auto triangles = std::vector<Triangle>{
            {rightTopFront, right, uv0, rightTopBack, right, uv0, rightBottomFront, right, uv0, material},
            {rightBottomBack, right, uv0, rightTopBack, right, uv0, rightBottomFront, right, uv0, material},
            {leftTopFront, left, uv0, leftTopBack, left, uv0, leftBottomFront, left, uv0, material},
            {leftBottomBack, left, uv0, leftTopBack, left, uv0, leftBottomFront, left, uv0, material},
            {rightTopFront, top, uv0, rightTopBack, top, uv0, leftTopFront, top, uv0, material},
            {leftTopBack, top, uv0, rightTopBack, top, uv0, leftTopFront, top, uv0, material},
            {rightBottomFront, bottom, uv0, rightBottomBack, bottom, uv0, leftBottomFront, bottom, uv0, material},
            {leftBottomBack, bottom, uv0, rightBottomBack, bottom, uv0, leftBottomFront, bottom, uv0, material},
            {rightTopFront, front, uv0, rightBottomFront, front, uv0, leftTopFront, front, uv0, material},
            {leftBottomFront, front, uv0, rightBottomFront, front, uv0, leftTopFront, front, uv0, material},
            {rightTopBack, back, uv0, rightBottomBack, back, uv0, leftTopBack, back, uv0, material},
            {leftBottomBack, back, uv0, rightBottomBack, back, uv0, leftTopBack, back, uv0, material},
        };
        _triangles = std::move(triangles);
        RebuildAlignedBox();
    }
};

#endif // MONTE_CARLO_RAY_TRACER_CUBE_HPP
