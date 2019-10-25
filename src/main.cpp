#include <functional>
#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <utility>
#include <vector>

#include "BoundedVolumnHierarchy/BvhNode.hpp"
#include "Camera.hpp"
#include "Hitables/Mesh.hpp"
#include "Hitables/Prefabs/Cube.hpp"
#include "Hitables/Prefabs/Rectangle.hpp"
#include "Hitables/Sphere.hpp"
#include "Materials/Lambertian.hpp"
#include "Materials/LightSource.hpp"
#include "Materials/Metal.hpp"
#include "Rendering/ConfigLoader.hpp"
#include "Rendering/RenderingEngine.hpp"
#include "Rendering/SceneLoader.hpp"
#include "Textures/ConstantColor.hpp"
#include "Utils.hpp"

using namespace std;

int main() {
    const auto hitables = SceneLoader::LoadScene("../../data/testScene.json");
    const auto configAndCamera = ConfigLoader::LoadConfig("../../data/testScene.json");
    const auto camera = configAndCamera.second;
    const auto width = configAndCamera.first.GetImageWidth();
    const auto height = configAndCamera.first.GetImageHeight();
    const auto maxDepth = configAndCamera.first.GetRayMaxDepth();
    const auto numSamples = configAndCamera.first.GetSamplesPerPixel();
    const auto bvh = BVH::BuildBVH(hitables);

    auto data = std::vector<unsigned char>();
    data.reserve(height * width * 3);
    auto dataSum = std::vector<unsigned long long>();
    dataSum.reserve(height * width * 3);

    for (auto n = 0; n < numSamples; n++) {
        for (auto i = 0; i < width; i++) {
#pragma omp parallel for default(none) shared(maxDepth, height, width, i, n, camera, bvh, dataSum, data)
            for (auto j = 0; j < height; j++) {
                const auto x = Utils::RandomFloatBetween((i - 0.5f) / width, (i + 0.5f) / width);
                const auto y = Utils::RandomFloatBetween((j - 0.5f) / height, (j + 0.5f) / height);
                Ray ray = camera.GetEmittedRay(x, y);
                const auto color = RenderingEngine::GetRayColor(bvh, ray, maxDepth);
                const auto index = ((height - 1 - j) * width + i) * 3;
                dataSum[index + 0] += std::clamp(color.b * 255, 0.0f, 255.0f);
                dataSum[index + 1] += std::clamp(color.g * 255, 0.0f, 255.0f);
                dataSum[index + 2] += std::clamp(color.r * 255, 0.0f, 255.0f);
                data[index + 0] = dataSum[index + 0] / (n + 1);
                data[index + 1] = dataSum[index + 1] / (n + 1);
                data[index + 2] = dataSum[index + 2] / (n + 1);
            }
        }

        const auto mat = cv::Mat(height, width, CV_8UC3, data.data()); // NOLINT(hicpp-signed-bitwise)
        cv::imshow("display", mat);
        cv::waitKey(1);
    }

    cv::waitKey(0);
    return 0;
}
