//
// Created by zhang on 10/25/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_SCENELOADER_HPP
#define MONTE_CARLO_RAY_TRACER_SCENELOADER_HPP

#include <fstream>
#include <memory>
#include <vector>

#include <json/json.hpp>

#include "../Hitables/Hitable.hpp"
#include "../Textures/ConstantColor.hpp"

using json = nlohmann::json;

class SceneLoader {
  public:
    static std::vector<std::shared_ptr<const Hitable>> LoadScene(const std::string &filePath) {
        std::ifstream t(filePath);
        json content;
        t >> content;

        // build textures
        std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
        for (const auto &textureJson : content["textures"]) {
            textures.insert(LoadTexture(textureJson));
        }

        // build materials
        std::unordered_map<std::string, std::shared_ptr<Material>> materials;
        for (const auto &materialJson : content["materials"]) {
            materials.insert(LoadMaterial(textures, materialJson));
        }

        // build hitables
        std::vector<std::shared_ptr<const Hitable>> hitables;
        for (const auto &hitableJson : content["hitables"]) {
            hitables.emplace_back(LoadHitable(materials, hitableJson));
        }

        return hitables;
    }

  private:
    static std::pair<std::string, std::shared_ptr<Texture>> LoadTexture(const json &textureJson) {
        const std::string type = textureJson["type"];
        const std::string name = textureJson["name"];
        if (type == "constantColor") {
            const auto &colorArray = textureJson["color"];
            Color color{colorArray[0], colorArray[1], colorArray[2]};
            return {name, std::make_shared<ConstantColor>(color)};
        }
        throw std::runtime_error("unknown texture type");
    }

    static std::pair<std::string, std::shared_ptr<Material>>
    LoadMaterial(const std::unordered_map<std::string, std::shared_ptr<Texture>> &textureMap,
                 const json &materialJson) {
        const std::string type = materialJson["type"];
        const std::string name = materialJson["name"];
        if (type == "lambertian") {
            const std::string textureName = materialJson["texture"];
            const auto &texture = textureMap.at(textureName);
            return {name, std::make_shared<Lambertian>(texture)};
        }
        throw std::runtime_error("unknown material type");
    }

    static std::shared_ptr<Hitable>
    LoadHitable(const std::unordered_map<std::string, std::shared_ptr<Material>> &materialMap,
                const json &hitableJson) {
        const std::string type = hitableJson["type"];
        const std::string materialName = hitableJson["material"];
        const auto &material = materialMap.at(materialName);
        if (type == "sphere") {
            const auto &center = hitableJson["center"];
            const float radius = hitableJson["radius"];
            return std::make_shared<Sphere>(Location(center[0], center[1], center[2]), radius, material);
        } else if (type == "mesh") {
            const std::string objPath = hitableJson["objPath"];
            const auto object = std::make_shared<Mesh>(objPath, material);
            for (const auto &transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(object, transformation);
            }
            return object;
        } else if (type == "prefabs.cube") {
            const auto cube = std::make_shared<Cube>(material);
            for (const auto &transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(cube, transformation);
            }
            return cube;
        } else if (type == "prefabs.rectangle") {
            const auto rectangle = std::make_shared<Rectangle>(hitableJson["height"], hitableJson["width"], material);
            for (const auto &transformation : hitableJson["transformations"]) {
                LoadAndPerformTransformation(rectangle, transformation);
            }
            return rectangle;
        }
        throw std::runtime_error("unknown hitable type");
    }

    static void LoadAndPerformTransformation(const std::shared_ptr<Mesh> &object, const json &transformationJson) {
        const std::string type = transformationJson["type"];
        if (type == "translation") {
            object->ApplyTranslation(transformationJson["dx"], transformationJson["dy"], transformationJson["dz"]);
            return;
        } else if (type == "rotation") {
            object->ApplyRotation(transformationJson["degrees"], transformationJson["x"], transformationJson["y"],
                                  transformationJson["z"]);
            return;
        } else if (type == "scaling") {
            object->ApplyScaling(transformationJson["scale"]);
            return;
        }
        throw std::runtime_error("unknown transformation type");
    }
};

#endif // MONTE_CARLO_RAY_TRACER_SCENELOADER_HPP
