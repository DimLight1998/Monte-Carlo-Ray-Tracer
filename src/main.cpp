#include <vector>
#include <iostream>
#include <functional>
#include <utility>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


#include "Utils.hpp"
#include "Materials/Lambertian.hpp"
#include "Materials/LightSource.hpp"
#include "Materials/Metal.hpp"
#include "Textures/ConstantColor.hpp"
#include "BoundedVolumnHierarchy/BvhNode.hpp"
#include "Hitables/Sphere.hpp"
#include "Hitables/Mesh.hpp"
#include "Hitables/Prefabs/Cube.hpp"
#include "Hitables/Prefabs/Rectangle.hpp"
#include "Camera.hpp"
#include "Rendering/RenderingEngine.hpp"
#include "Rendering/SceneLoader.hpp"

using namespace std;

int main() {
    SceneLoader::LoadScene("../../data/testScene.json");
//    cout << 1;
//    vector<Sphere> balls;
//    ConstantColor white({0.9, 0.9, 0.9});
//    ConstantColor dark({0.3, 0.3, 0.3});
//    ConstantColor gray({0.7, 0.7, 0.7});
//    ConstantColor orange({255.0 * 10 / 255.0, 193.0 * 10 / 255.0, 7.0 * 10 / 255.0});
//    Lambertian lamWhite(white);
//    Lambertian lamDark(dark);
//    Metal metalDark({0.3, 0.3, 0.3}, 0.7f);
//    Lambertian lamGray(gray);
//    LightSource orangeLight(orange);
//
//    balls.emplace_back(Sphere({0, -1000, 0}, 1000, metalDark));
////    balls.emplace_back(Sphere({0, 3, 0}, 3, lamGray));
////    balls.emplace_back(Sphere({-2.5, 4, -6}, 2.5, lamGray));
////    balls.emplace_back(Sphere({6, 4, 0}, 2, lamGray));
////    balls.emplace_back(Sphere({8, 3, 3}, 1.5, lamGray));
////    balls.emplace_back(Sphere({10, 5, -5}, 1, lamGray));
//
////    Mesh bunny("bunny.obj", lamGray);
////    bunny.ApplyScaling(10);
////    bunny.ApplyRotation(180, 0, 2, 0);
//
////    Cube cube(lamWhite);
////    cube.ApplyScaling(2);
////    cube.ApplyTranslation(0, 3, 0);
////    cube.ApplyRotation(0.3, 0.1, 0.1, 0.1);
//
//    Rectangle rect(1, 1, lamWhite);
//    rect.ApplyScaling(5).ApplyRotation(0.1, 1, 1, 1);
//
//    Rectangle leftWall(1, 1, lamWhite);
//    leftWall.ApplyScaling(10).ApplyRotation(glm::pi<float>() / 2, 0, 1, 0).ApplyTranslation(6, 0, 0);
//
//    vector<reference_wrapper<const Hitable>> hitables;
//    for_each(balls.begin(), balls.end(), [&](const auto &ball) {
//        hitables.emplace_back(ball);
//    });
////    hitables.emplace_back(cube);
////    hitables.emplace_back(rect);
//    hitables.emplace_back(leftWall);
//
//    const BVH &bvh = BVH::BuildBVH(hitables);
//
//    const auto edge = 400;
//
//    auto data = std::array<unsigned char, edge * edge * 3>{};
//    auto dataSum = std::array<unsigned long long, edge * edge * 3>{};
//    Camera cam({0, 15, -50}, {0, 0, 0}, {0, 1, 0}, 30, 1, 0, 10, 0, 1);
//    for (auto n = 0; n < 100; n++) {
//        for (auto i = 0; i < edge; i++) {
//            #pragma omp parallel for default(none) shared(i, n, cam, bvh, dataSum, data)
//            for (auto j = 0; j < edge; j++) {
//                const auto x = Utils::RandomFloatBetween((i - 0.5f) / edge, (i + 0.5f) / edge);
//                const auto y = Utils::RandomFloatBetween((j - 0.5f) / edge, (j + 0.5f) / edge);
//                Ray ray = cam.GetEmittedRay(x, y);
//                const auto color = RenderingEngine::GetRayColor(bvh, ray, 10);
//                const auto index = ((edge - 1 - j) * edge + i) * 3;
//                dataSum[index + 0] += std::clamp(color.b * 255, 0.0f, 255.0f);
//                dataSum[index + 1] += std::clamp(color.g * 255, 0.0f, 255.0f);
//                dataSum[index + 2] += std::clamp(color.r * 255, 0.0f, 255.0f);
//                data[index + 0] = dataSum[index + 0] / (n + 1);
//                data[index + 1] = dataSum[index + 1] / (n + 1);
//                data[index + 2] = dataSum[index + 2] / (n + 1);
//            }
//        }
//
//        const auto mat = cv::Mat(edge, edge, CV_8UC3, data.data()); // NOLINT(hicpp-signed-bitwise)
//        cv::imshow("display", mat);
//        cv::waitKey(1);
//    }
//
//    cv::waitKey(0);
    return 0;
}
