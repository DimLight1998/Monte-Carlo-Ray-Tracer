//
// Created by zhang on 10/25/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_SCENELOADER_HPP
#define MONTE_CARLO_RAY_TRACER_SCENELOADER_HPP


#include <vector>
#include <memory>
#include <fstream>


#include <json/json.hpp>

#include "../Hitables/Hitable.hpp"

using json = nlohmann::json;

class SceneLoader {
public:
    static std::vector<std::unique_ptr<Hitable>> LoadScene(const std::string &filePath) {
        std::ifstream t(filePath);
        json content;
        t >> content;

        // build textures
        std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
        for (const auto &textureJson : content["textures"]) {
            textures.insert(LoadTexture(textureJson));
        }

        // build materials
        std::unordered_map<std::string, std::unique_ptr<Material>> materials;
        for (const auto &materialJson : content["materials"]) {
            materials.insert(LoadMaterial(textures, materialJson));
        }

        // build hitables
        std::vector<std::unique_ptr<Hitable>> hitables;
        for (const auto &hitableJson : content["hitables"]) {
            hitables.emplace_back(LoadHitable(materials, hitableJson));
        }

        return hitables;
    }

private:
    static std::pair<std::string, std::unique_ptr<Texture>> LoadTexture(const json &textureJson) {
        const std::string type = textureJson["type"];
        const std::string name = textureJson["name"];
        if (type == "constantColor") {
            const auto &colorArray = textureJson["color"];
            Color color{colorArray[0], colorArray[1], colorArray[2]};
            return {name, std::make_unique<ConstantColor>(color)};
        }
        throw std::runtime_error("unknown texture type");
    }

    static std::pair<std::string, std::unique_ptr<Material>> LoadMaterial(
        const std::unordered_map<std::string, std::unique_ptr<Texture>> &textureMap,
        const json &materialJson
    ) {
        const std::string type = materialJson["type"];
        const std::string name = materialJson["name"];
        if (type == "lambertian") {
            const std::string textureName = materialJson["texture"];
            const auto &texture = textureMap.at(textureName);
            return {name, std::make_unique<Lambertian>(*texture)};
        }
        throw std::runtime_error("unknown material type");
    }

    static std::unique_ptr<Hitable> LoadHitable(
        const std::unordered_map<std::string, std::unique_ptr<Material>> &materialMap,
        const json &hitableJson
    ) {
        const std::string type = hitableJson["type"];
        if (type == "sphere") {
            const auto &center = hitableJson["center"];
            const float radius = hitableJson["radius"];
            const std::string materialName = hitableJson["material"];
            const auto &material = materialMap.at(materialName);
            return std::make_unique<Sphere>(Location(center[0], center[1], center[2]), radius, *material);
        }
        throw std::runtime_error("unknown hitable type");
    }
};


#endif //MONTE_CARLO_RAY_TRACER_SCENELOADER_HPP
