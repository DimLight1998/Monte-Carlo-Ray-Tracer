//
// Created by zhang on 10/25/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_CONFIGLOADER_HPP
#define MONTE_CARLO_RAY_TRACER_CONFIGLOADER_HPP

#include "../Camera.hpp"
#include <fstream>
#include <json/json.hpp>

using json = nlohmann::json;

class ImageConfig {
  public:
    ImageConfig(int imageWidth, int imageHeight, int rayMaxDepth, int samplesPerPixel)
        : _imageWidth(imageWidth), _imageHeight(imageHeight), _rayMaxDepth(rayMaxDepth),
          _samplesPerPixel(samplesPerPixel) {}
    int GetImageWidth() const { return _imageWidth; }
    int GetImageHeight() const { return _imageHeight; }
    int GetRayMaxDepth() const { return _rayMaxDepth; }
    int GetSamplesPerPixel() const { return _samplesPerPixel; }

  private:
    int _imageWidth;
    int _imageHeight;
    int _rayMaxDepth;
    int _samplesPerPixel;
};

class ConfigLoader {
  public:
    static std::pair<ImageConfig, Camera> LoadConfig(const std::string &filePath) {
        std::ifstream t(filePath);
        json content;
        t >> content;

        const int imageWidth = content["settings"]["imageWidth"];
        const int imageHeight = content["settings"]["imageHeight"];
        const int rayMaxDepth = content["settings"]["rayMaxDepth"];
        const int samplesPerPixel = content["settings"]["samplesPerPixel"];
        ImageConfig config(imageWidth, imageHeight, rayMaxDepth, samplesPerPixel);

        const auto readVec3 = [](const json &j) { return glm::vec3(j[0], j[1], j[2]); };
        const auto lookFrom = readVec3(content["camera"]["lookFrom"]);
        const auto lookAt = readVec3(content["camera"]["lookAt"]);
        const auto upDirection = readVec3(content["camera"]["upDirection"]);
        const float verticalFov = content["camera"]["verticalFov"];
        const float aspect = content["camera"]["aspect"];
        const float aperture = content["camera"]["aperture"];
        const float focusDistance = content["camera"]["focusDistance"];
        const float shutterStart = content["camera"]["shutterStart"];
        const float shutterEnd = content["camera"]["shutterEnd"];

        Camera camera(lookFrom, lookAt, upDirection, verticalFov, aspect, aperture, focusDistance, shutterStart,
                      shutterEnd);
        return std::make_pair(config, camera);
    }
};

#endif // MONTE_CARLO_RAY_TRACER_CONFIGLOADER_HPP
