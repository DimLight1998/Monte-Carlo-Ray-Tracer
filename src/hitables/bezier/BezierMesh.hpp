//
// Created by zhang on 12/15/19.
//

#ifndef MONTECARLORAYTRACER_BEZIERMESH_HPP
#define MONTECARLORAYTRACER_BEZIERMESH_HPP

#include <iostream>

#include "../Mesh.hpp"
#include "Bezier.hpp"

class BezierMesh: public Mesh {
    public:
    explicit BezierMesh(
        const std::vector<std::vector<std::vector<glm::vec3>>>& patchesData,
        const std::shared_ptr<Material>&                        material,
        float                                                   error) {
        Bezier bezier(patchesData);
        bezier.ApplyAdaptiveSubdivision(error);
        _triangles = std::move(bezier.ToTriangles(material));
        std::cout << _triangles.size() << " triangles of the bezier object generated." << std::endl;
        RebuildAlignedBox();
    }
};

#endif  //MONTECARLORAYTRACER_BEZIERMESH_HPP
