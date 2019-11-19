//
// Created by zhang on 10/25/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_SCENELOADER_HPP
#define MONTE_CARLO_RAY_TRACER_SCENELOADER_HPP

#include <fstream>
#include <memory>
#include <vector>

#include <json/json.hpp>

#include "../hitables/Hitable.hpp"
#include "../hitables/Mesh.hpp"
#include "../hitables/SmokeWrapper.hpp"
#include "../hitables/Sphere.hpp"
#include "../hitables/prefabs/Cube.hpp"
#include "../hitables/prefabs/Rectangle.hpp"
#include "../materials/Glass.hpp"
#include "../materials/Lambertian.hpp"
#include "../materials/LightSource.hpp"
#include "../materials/Metal.hpp"
#include "../materials/Smoke.hpp"
#include "../textures/ConstantColor.hpp"
#include "../textures/DiffuseMapping.hpp"

using json = nlohmann::json;

class SceneLoader {
    public:
    static std::vector<std::shared_ptr<const Hitable>> LoadScene(const std::string& filePath) {
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
        std::vector<std::shared_ptr<const Hitable>> hitables;
        for (const auto& hitableJson : content["hitables"]) {
            hitables.emplace_back(LoadHitable(materials, hitableJson));
        }

        return hitables;
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

    static std::shared_ptr<Hitable> LoadHitable(
        const std::unordered_map<std::string, std::shared_ptr<Material>>& materialMap,
        const json&                                                       hitableJson) {
        const std::string type         = hitableJson["type"];
        const std::string materialName = hitableJson["material"];
        const auto&       material     = materialMap.at(materialName);
        if (type == "sphere") {
            const auto& center = hitableJson["center"];
            const float radius = hitableJson["radius"];
            return std::make_shared<Sphere>(Location(center[0], center[1], center[2]), radius, material);
        } else if (type == "mesh") {
            const std::string objPath = hitableJson["objPath"];
            const auto        object  = std::make_shared<Mesh>(objPath, material);
            for (const auto& transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(object, transformation);
            }
            return object;
        } else if (type == "prefabs.cube") {
            const float x    = hitableJson["x"];
            const float y    = hitableJson["y"];
            const float z    = hitableJson["z"];
            const auto  cube = std::make_shared<Cube>(material, x, y, z);
            for (const auto& transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(cube, transformation);
            }
            return cube;
        } else if (type == "prefabs.rectangle") {
            const auto rectangle = std::make_shared<Rectangle>(hitableJson["height"], hitableJson["width"], material);
            for (const auto& transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(rectangle, transformation);
            }
            return rectangle;
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
            return rectangle;
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
            return rectangle;
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
            return rectangle;
        } else if (type == "smokeWrapper") {
            const auto wrapped       = LoadHitable(materialMap, hitableJson["wrapping"]);
            const auto smokeMaterial = std::dynamic_pointer_cast<Smoke>(material);
            if (smokeMaterial == nullptr) throw std::runtime_error("smoke wrapper with material not being smoke");
            return std::make_shared<SmokeWrapper>(wrapped, smokeMaterial);
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
