//
// Created by zhang on 10/25/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_SCENELOADER_HPP
#define MONTE_CARLO_RAY_TRACER_SCENELOADER_HPP

#include <fstream>
#include <memory>
#include <tuple>
#include <vector>

#include <json/json.hpp>

#include "../hitables/Hitable.hpp"
#include "../hitables/Mesh.hpp"
#include "../hitables/MovingSphere.hpp"
#include "../hitables/SmokeWrapper.hpp"
#include "../hitables/Sphere.hpp"
#include "../hitables/bezier/BezierMesh.hpp"
#include "../hitables/prefabs/Cube.hpp"
#include "../hitables/prefabs/Rectangle.hpp"
#include "../materials/Glass.hpp"
#include "../materials/Lambertian.hpp"
#include "../materials/LightSource.hpp"
#include "../materials/Metal.hpp"
#include "../materials/Smoke.hpp"
#include "../textures/ConstantColor.hpp"
#include "../textures/DiffuseMapping.hpp"
#include "../textures/PerlinNoise.hpp"

using json = nlohmann::json;

class SceneLoader {
    public:
    static std::tuple<
        std::vector<std::shared_ptr<const Hitable>>,
        std::vector<std::shared_ptr<const Hitable>>,
        std::vector<std::shared_ptr<const Hitable>>>
    LoadScene(const std::string& filePath) {
        std::ifstream t(filePath);
        json          content;
        t >> content;

        // build textures
        std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
        for (const auto& textureJson : content["textures"]) {
            textures.insert(LoadTexture(textureJson));
        }

        // build materials
        std::unordered_map<std::string, std::shared_ptr<Material>> materials;
        for (const auto& materialJson : content["materials"]) {
            materials.insert(LoadMaterial(textures, materialJson));
        }

        // build hitables
        std::vector<std::shared_ptr<const Hitable>> allHitables;
        std::vector<std::shared_ptr<const Hitable>> mcImportantHitables;
        std::vector<std::shared_ptr<const Hitable>> pmImportantHitables;
        for (const auto& hitableJson : content["hitables"]) {
            const auto  hitableInfo   = LoadHitable(materials, hitableJson);
            const auto& hitable       = std::get<0>(hitableInfo);
            const auto  isMCImportant = std::get<1>(hitableInfo);
            const auto  isPMImportant = std::get<2>(hitableInfo);

            allHitables.emplace_back(hitable);
            if (isMCImportant) mcImportantHitables.emplace_back(hitable);
            if (isPMImportant) pmImportantHitables.emplace_back(hitable);
        }

        return { allHitables, mcImportantHitables, pmImportantHitables };
    }

    private:
    static std::pair<std::string, std::shared_ptr<Texture>> LoadTexture(const json& textureJson) {
        const std::string type = textureJson["type"];
        const std::string name = textureJson["name"];
        if (type == "constantColor") {
            const auto& colorArray = textureJson["color"];
            Color       color { colorArray[0], colorArray[1], colorArray[2] };
            return { name, std::make_shared<ConstantColor>(color) };
        } else if (type == "diffuseMapping") {
            const std::string imagePath = textureJson["imagePath"];
            return { name, std::make_shared<DiffuseMapping>(imagePath) };
        } else if (type == "perlinNoise") {
            const float scale           = textureJson["scale"];
            const float depth           = textureJson["depth"];
            const float sinTurbCoeff    = textureJson["sinTurbCoeff"];
            const float sinOffset       = textureJson["sinOffset"];
            const float sinXCoeff       = textureJson["sinXCoeff"];
            const float sinYCoeff       = textureJson["sinYCoeff"];
            const float sinZCoeff       = textureJson["sinZCoeff"];
            const float sinXYCoeff      = textureJson["sinXYCoeff"];
            const float sinYZCoeff      = textureJson["sinYZCoeff"];
            const float sinZXCoeff      = textureJson["sinZXCoeff"];
            const float sinXYZCoeff     = textureJson["sinXYZCoeff"];
            const float sinCoeff        = textureJson["sinCoeff"];
            const float linearTurbCoeff = textureJson["linearTurbCoeff"];
            const float linearOffset    = textureJson["linearOffset"];
            const auto& color           = textureJson["color"];
            return {
                name, std::make_shared<PerlinNoise>(
                          scale,
                          depth,
                          sinTurbCoeff,
                          sinOffset,
                          sinXCoeff,
                          sinYCoeff,
                          sinZCoeff,
                          sinXYCoeff,
                          sinYZCoeff,
                          sinZXCoeff,
                          sinXYZCoeff,
                          sinCoeff,
                          linearTurbCoeff,
                          linearOffset,
                          Color { color[0], color[1], color[2] })
            };
        }
        throw std::runtime_error("unknown texture type");
    }

    static std::pair<std::string, std::shared_ptr<Material>> LoadMaterial(
        const std::unordered_map<std::string, std::shared_ptr<Texture>>& textureMap,
        const json&                                                      materialJson) {
        const std::string type = materialJson["type"];
        const std::string name = materialJson["name"];
        if (type == "lambertian") {
            const std::string textureName = materialJson["texture"];
            const auto&       texture     = textureMap.at(textureName);
            return { name, std::make_shared<Lambertian>(texture) };
        } else if (type == "metal") {
            const auto& attenuationArray = materialJson["attenuation"];
            Color       attenuation { attenuationArray[0], attenuationArray[1], attenuationArray[2] };
            const float fuzziness = materialJson["fuzziness"];
            return { name, std::make_shared<Metal>(attenuation, fuzziness) };
        } else if (type == "glass") {
            const auto& attenuationArray = materialJson["attenuation"];
            Color       attenuation { attenuationArray[0], attenuationArray[1], attenuationArray[2] };
            const float index = materialJson["index"];
            return { name, std::make_shared<Glass>(index, attenuation) };
        } else if (type == "lightSource") {
            const std::string textureName = materialJson["texture"];
            const auto&       texture     = textureMap.at(textureName);
            return { name, std::make_shared<LightSource>(texture) };
        } else if (type == "smoke") {
            const std::string textureName = materialJson["texture"];
            const auto&       texture     = textureMap.at(textureName);
            const float       density     = materialJson["density"];
            return { name, std::make_shared<Smoke>(texture, density) };
        }
        throw std::runtime_error("unknown material type");
    }

    static std::tuple<std::shared_ptr<Hitable>, bool, bool> LoadHitable(
        const std::unordered_map<std::string, std::shared_ptr<Material>>& materialMap,
        const json&                                                       hitableJson) {
        const std::string type         = hitableJson["type"];
        const std::string materialName = hitableJson["material"];
        const bool        mcImportant  = hitableJson.value("mcImportant", false);
        const bool        pmImportant  = hitableJson.value("pmImportant", false);
        const auto&       material     = materialMap.at(materialName);
        if (type == "sphere") {
            const auto& center = hitableJson["center"];
            const float radius = hitableJson["radius"];
            return { std::make_shared<Sphere>(Location(center[0], center[1], center[2]), radius, material),
                     mcImportant,
                     pmImportant };
        } else if (type == "movingSphere") {
            const auto& centerStart = hitableJson["centerStart"];
            const auto& centerEnd   = hitableJson["centerEnd"];
            const float radius      = hitableJson["radius"];
            const float movingStart = hitableJson["movingStart"];
            const float movingEnd   = hitableJson["movingEnd"];
            return { std::make_shared<MovingSphere>(
                         Location { centerStart[0], centerStart[1], centerStart[2] },
                         Location { centerEnd[0], centerEnd[1], centerEnd[2] },
                         radius,
                         movingStart,
                         movingEnd,
                         material),
                     false,
                     false };
        } else if (type == "mesh") {
            const std::string objPath = hitableJson["objPath"];
            const auto        object  = std::make_shared<Mesh>(objPath, material);
            for (const auto& transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(object, transformation);
            }
            return { object, mcImportant, pmImportant };
        } else if (type == "bezier") {
            const auto  data  = hitableJson["data"];
            const float error = hitableJson["error"];

            std::vector<std::vector<std::vector<glm::vec3>>> controlData;
            for (const auto& surface : data) {
                std::vector<std::vector<glm::vec3>> surfaceData;
                for (const auto& row : surface) {
                    std::vector<glm::vec3> rowData;
                    for (const auto& point : row) {
                        rowData.emplace_back(glm::vec3 { point[0], point[1], point[2] });
                    }
                    surfaceData.emplace_back(rowData);
                }
                controlData.emplace_back(surfaceData);
            }
            const auto bezier = std::make_shared<BezierMesh>(controlData, material, error);
            for (const auto& transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(bezier, transformation);
            }
            return { bezier, mcImportant, pmImportant };
        } else if (type == "prefabs.cube") {
            const float x    = hitableJson["x"];
            const float y    = hitableJson["y"];
            const float z    = hitableJson["z"];
            const auto  cube = std::make_shared<Cube>(material, x, y, z);
            for (const auto& transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(cube, transformation);
            }
            return { cube, mcImportant, pmImportant };
        } else if (type == "prefabs.rectangle") {
            const auto rectangle = std::make_shared<Rectangle>(hitableJson["height"], hitableJson["width"], material);
            for (const auto& transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(rectangle, transformation);
            }
            return { rectangle, mcImportant, pmImportant };
        } else if (type == "prefabs.rectangle:xy+" || type == "prefabs.rectangle:xy-") {
            const float xMin         = hitableJson["xMin"];
            const float xMax         = hitableJson["xMax"];
            const float yMin         = hitableJson["yMin"];
            const float yMax         = hitableJson["yMax"];
            const float z            = hitableJson["z"];
            const auto  positiveNorm = *(type.end() - 1) == '+';
            const auto  rectangle =
                std::make_shared<Rectangle>(Rectangle::OfXY(xMin, xMax, yMin, yMax, z, positiveNorm, material));
            for (const auto& transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(rectangle, transformation);
            }
            return { rectangle, mcImportant, pmImportant };
        } else if (type == "prefabs.rectangle:+yz" || type == "prefabs.rectangle:-yz") {
            const float yMin         = hitableJson["yMin"];
            const float yMax         = hitableJson["yMax"];
            const float zMin         = hitableJson["zMin"];
            const float zMax         = hitableJson["zMax"];
            const float x            = hitableJson["x"];
            const auto  positiveNorm = *(type.end() - 3) == '+';
            const auto  rectangle =
                std::make_shared<Rectangle>(Rectangle::OfYZ(yMin, yMax, zMin, zMax, x, positiveNorm, material));
            for (const auto& transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(rectangle, transformation);
            }
            return { rectangle, mcImportant, pmImportant };
        } else if (type == "prefabs.rectangle:x+z" || type == "prefabs.rectangle:x-z") {
            const float zMin         = hitableJson["zMin"];
            const float zMax         = hitableJson["zMax"];
            const float xMin         = hitableJson["xMin"];
            const float xMax         = hitableJson["xMax"];
            const float y            = hitableJson["y"];
            const auto  positiveNorm = *(type.end() - 2) == '+';
            const auto  rectangle =
                std::make_shared<Rectangle>(Rectangle::OfZX(zMin, zMax, xMin, xMax, y, positiveNorm, material));
            for (const auto& transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(rectangle, transformation);
            }
            return { rectangle, mcImportant, pmImportant };
        } else if (type == "smokeWrapper") {
            const auto wrapped       = std::get<0>(LoadHitable(materialMap, hitableJson["wrapping"]));
            const auto smokeMaterial = std::dynamic_pointer_cast<Smoke>(material);
            if (smokeMaterial == nullptr) throw std::runtime_error("smoke wrapper with material not being smoke");
            const auto smokeWrapper = std::make_shared<SmokeWrapper>(wrapped, smokeMaterial);
            return { smokeWrapper, mcImportant, pmImportant };
        }
        throw std::runtime_error("unknown hitable type");
    }

    static void LoadAndPerformTransformation(const std::shared_ptr<Mesh>& object, const json& transformationJson) {
        const std::string type = transformationJson["type"];
        if (type == "translation") {
            object->ApplyTranslation(transformationJson["dx"], transformationJson["dy"], transformationJson["dz"]);
            return;
        } else if (type == "rotation") {
            object->ApplyRotation(
                transformationJson["degrees"],
                transformationJson["x"],
                transformationJson["y"],
                transformationJson["z"]);
            return;
        } else if (type == "scaling") {
            object->ApplyScaling(transformationJson["scale"]);
            return;
        }
        throw std::runtime_error("unknown transformation type");
    }
};

#endif  // MONTE_CARLO_RAY_TRACER_SCENELOADER_HPP
