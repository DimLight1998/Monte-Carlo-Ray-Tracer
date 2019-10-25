//
// Created by zhang on 10/23/19.
//

#include <vector>
#include <functional>

#include "Bvh.hpp"
#include "../Hitables/Hitable.hpp"
#include "BvhEmpty.hpp"
#include "BvhNode.hpp"
#include "../Utils.hpp"

const BVH &BVH::BuildBVH(const std::vector<std::reference_wrapper<const Hitable>> &hitables) {
    auto copy = hitables;
    return BVH::BuildBVH(copy, 0, static_cast<int>(hitables.size()));
}

const BVH &BVH::BuildBVH(std::vector<std::reference_wrapper<const Hitable>> &hitables, int lo, int hi) {
    if (lo == hi) {
        return *new BVHEmpty();
    } else if (lo + 1 == hi) {
        const auto &item = hitables[lo].get();
        return item.BuildBVH();
    } else {
        const auto axisIndex = Utils::RandomIntBetween(0, 2);
        if (axisIndex == 0) {
            std::sort(
                hitables.begin() + lo, hitables.begin() + hi,
                [](const auto &h1, const auto &h2) {
                    return h1.get().GetAlignedBox().GetXMin() < h2.get().GetAlignedBox().GetXMin();
                });
        } else if (axisIndex == 1) {
            std::sort(
                hitables.begin() + lo, hitables.begin() + hi,
                [](const auto &h1, const auto &h2) {
                    return h1.get().GetAlignedBox().GetYMin() < h2.get().GetAlignedBox().GetYMin();
                });
        } else if (axisIndex == 2) {
            std::sort(
                hitables.begin() + lo, hitables.begin() + hi,
                [](const auto &h1, const auto &h2) {
                    return h1.get().GetAlignedBox().GetZMin() < h2.get().GetAlignedBox().GetZMin();
                });
        } else {
            throw std::runtime_error("impossible");
        }
        const auto mid = (lo + hi) / 2;
        const auto &bvh1 = BuildBVH(hitables, lo, mid);
        const auto &bvh2 = BuildBVH(hitables, mid, hi);
        return *new BVHNode(bvh1, bvh2);
    }
}
