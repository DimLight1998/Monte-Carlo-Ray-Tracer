#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "Rendering/ConfigLoader.hpp"
#include "Rendering/RenderingEngine.hpp"
#include "Rendering/SceneLoader.hpp"

using namespace std;

int main() {
    const auto hitables        = SceneLoader::LoadScene("../../data/testScene.json");
    const auto configAndCamera = ConfigLoader::LoadConfig("../../data/testScene.json");
    const auto camera          = configAndCamera.second;
    const auto width           = configAndCamera.first.GetImageWidth();
    const auto height          = configAndCamera.first.GetImageHeight();
    const auto maxDepth        = configAndCamera.first.GetRayMaxDepth();
    const auto numSamples      = configAndCamera.first.GetSamplesPerPixel();
    const auto skyColor        = configAndCamera.first.GetSkyColor();
    const auto bvh             = BVH::BuildBVH(hitables);

    auto data = std::vector<unsigned char>();
    data.reserve(height * width * 3);
    auto dataSum = std::vector<long double>();
    dataSum.reserve(height * width * 3);

    for (auto n = 0; n < numSamples; n++) {
        for (auto i = 0; i < width; i++) {
#pragma omp parallel for default(none) shared(maxDepth, height, width, i, n, camera, bvh, dataSum, data, skyColor)
            for (auto j = 0; j < height; j++) {
                const auto x     = RandomFloatBetween((i - 0.5f) / width, (i + 0.5f) / width);
                const auto y     = RandomFloatBetween((j - 0.5f) / height, (j + 0.5f) / height);
                Ray        ray   = camera.GetEmittedRay(x, y);
                const auto color = RenderingEngine::GetRayColor(bvh, ray, maxDepth, skyColor);
                const auto index = ((height - 1 - j) * width + i) * 3;

                dataSum[index + 0] += color.b;
                dataSum[index + 1] += color.g;
                dataSum[index + 2] += color.r;
                data[index + 0] = std::clamp(glm::sqrt(dataSum[index + 0] / (n + 1)) * 256, 0.0l, 255.99l);
                data[index + 1] = std::clamp(glm::sqrt(dataSum[index + 1] / (n + 1)) * 256, 0.0l, 255.99l);
                data[index + 2] = std::clamp(glm::sqrt(dataSum[index + 2] / (n + 1)) * 256, 0.0l, 255.99l);
            }
        }

        const auto mat = cv::Mat(height, width, CV_8UC3, data.data());  // NOLINT(hicpp-signed-bitwise)
        cv::imshow("display", mat);
        cv::waitKey(1);
    }

    const auto         t  = std::time(nullptr);
    const auto         tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S");
    const auto timeString = oss.str();
    const auto mat        = cv::Mat(height, width, CV_8UC3, data.data());  // NOLINT(hicpp-signed-bitwise)
    cv::imwrite(timeString + ".png", mat);

    cv::waitKey(0);
    return 0;
}
