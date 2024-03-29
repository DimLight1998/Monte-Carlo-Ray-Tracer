#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "rendering/ConfigLoader.hpp"
#include "rendering/PhotonMapping.hpp"
#include "rendering/RenderingEngine.hpp"
#include "rendering/SceneLoader.hpp"

using namespace std;

int main() {
    const auto         timeStart      = std::time(nullptr);
    const auto         localTimeStart = *std::localtime(&timeStart);
    std::ostringstream ossStart;
    ossStart << std::put_time(&localTimeStart, "%Y-%m-%d-%H-%M-%S");
    const auto timeStringStart = ossStart.str();

    const auto  hitablesInfo        = SceneLoader::LoadScene("../../data/testScene.json");
    const auto  configAndCamera     = ConfigLoader::LoadConfig("../../data/testScene.json");
    const auto  camera              = configAndCamera.second;
    const auto  width               = configAndCamera.first.GetImageWidth();
    const auto  height              = configAndCamera.first.GetImageHeight();
    const auto  maxDepth            = configAndCamera.first.GetRayMaxDepth();
    const auto  numSamples          = configAndCamera.first.GetSamplesPerPixel();
    const auto  skyColor            = configAndCamera.first.GetSkyColor();
    const auto  bvh                 = BVH::BuildBVH(get<0>(hitablesInfo));
    const auto& mcImportantHitables = get<1>(hitablesInfo);
    const auto& pmImportantHitables = get<2>(hitablesInfo);

    PhotonMap photonMap;
    using DataBuffer = std::vector<std::pair<Location, Direction>>;
    DataBuffer totalDataBuffer;
    if (!pmImportantHitables.empty()) {
#pragma omp parallel for default(none) shared(bvh, totalDataBuffer, pmImportantHitables)
        for (auto i = 0; i < 16; i++) {
            DataBuffer subDataBuffer;
            for (auto j = 0; j < 400000; j++) {
                const auto ray = GetRandomRayFromPMHitables(pmImportantHitables);
                AppendPhotonMappingData(bvh, ray, 3, subDataBuffer);
            }
#pragma omp critical
            {
                for (const auto& item : subDataBuffer) {
                    totalDataBuffer.emplace_back(item);
                }
            }
        }
        for (const auto& item : totalDataBuffer) {
            photonMap.Add(item.first, item.second);
        }
    }
    photonMap.FreezeThenBuildTree();
    cout << "photon map built with " << photonMap.GetCount() << " photons" << endl;

    auto data = std::vector<unsigned char>();
    data.reserve(height * width * 3);
    auto dataSum = std::vector<long double>();
    dataSum.reserve(height * width * 3);

    for (auto n = 0; n < numSamples; n++) {
        for (auto i = 0; i < width; i++) {
#pragma omp parallel for default(none) \
    shared(maxDepth, height, width, i, n, camera, bvh, dataSum, data, skyColor, mcImportantHitables, photonMap)
            for (auto j = 0; j < height; j++) {
                const auto x   = RandomFloatBetween((i - 0.5f) / width, (i + 0.5f) / width);
                const auto y   = RandomFloatBetween((j - 0.5f) / height, (j + 0.5f) / height);
                Ray        ray = camera.GetEmittedRay(x, y);
                const auto color =
                    RenderingEngine::GetRayColor(bvh, ray, maxDepth, skyColor, mcImportantHitables, photonMap);
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
        cout << (n + 1) << " samples finished" << endl;

        const auto         timeNow      = std::time(nullptr);
        const auto         localTimeNow = *std::localtime(&timeNow);
        std::ostringstream ossNow;
        ossNow << std::put_time(&localTimeNow, "%Y-%m-%d-%H-%M-%S");
        const auto timeStringNow = ossNow.str();

        std::ostringstream title;
        title << timeStringStart << "-" << to_string(n + 1) << "-" << timeStringNow << ".png";
        cv::imwrite(title.str(), mat);
    }

    cv::waitKey(0);
    return 0;
}
