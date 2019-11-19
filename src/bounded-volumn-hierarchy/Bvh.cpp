//
// Created by zhang on 10/23/19.
//

#include <functional>
#include <vector>

#include "Bvh.hpp"
#include "BvhEmpty.hpp"
#include "BvhNode.hpp"

#include "../hitables/Hitable.hpp"

std::unique_ptr<BVH> BVH::BuildBVH(const std::vector<std::shared_ptr<const Hitable>>& hitables) {
    auto copy = hitables;
    return BVH::BuildBVH(copy, 0, static_cast<int>(hitables.size()));
}

std::unique_ptr<BVH> BVH::BuildBVH(std::vector<std::shared_ptr<const Hitable>>& hitables, int lo, int hi) {
    if (lo == hi) {
        return std::make_unique<BVHEmpty>();
    } else if (lo + 1 == hi) {
        const auto& item = hitables[lo].get();
        return item->BuildBVH();
    } else {
        const auto axisIndex = RandomIntegerBetween(0, 2);
        if (axisIndex == 0) {
            std::sort(hitables.begin() + lo, hitables.begin() + hi, [](const auto& h1, const auto& h2) {
                return h1->GetAlignedBox().GetXMin() < h2->GetAlignedBox().GetXMin();
            });
        } else if (axisIndex == 1) {
            std::sort(hitables.begin() + lo, hitables.begin() + hi, [](const auto& h1, const auto& h2) {
                return h1->GetAlignedBox().GetYMin() < h2->GetAlignedBox().GetYMin();
            });
        } else if (axisIndex == 2) {
            std::sort(hitables.begin() + lo, hitables.begin() + hi, [](const auto& h1, const auto& h2) {
                return h1->GetAlignedBox().GetZMin() < h2->GetAlignedBox().GetZMin();
            });
        } else {
            throw std::runtime_error("impossible");
        }
        const auto mid  = (lo + hi) / 2;
        auto       bvh1 = BuildBVH(hitables, lo, mid);
        auto       bvh2 = BuildBVH(hitables, mid, hi);
        return std::make_unique<BVHNode>(bvh1, bvh2);
    }
}
