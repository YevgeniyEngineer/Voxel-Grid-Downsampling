#include "voxel_grid_downsampling.hpp"
#include "xxhash.h"

#include <cmath>
#include <unordered_map>
#include <utility>

namespace downsampling
{
inline bool VoxelKey::operator==(const VoxelKey &other) const
{
    return (x == other.x) && (y == other.y) && (z == other.z);
}

inline std::size_t Hash::operator()(const VoxelKey &key) const
{
    return XXH64(static_cast<const void *>(&key), sizeof(VoxelKey), 0 /* seed */);
}

VoxelGridDownsampling::VoxelGridDownsampling(float leaf_size) : leaf_size_(leaf_size), inv_leaf_size_(1.0f / leaf_size_)
{
}

std::vector<Point> VoxelGridDownsampling::downsample(const std::vector<Point> &input_cloud)
{
    std::unordered_map<VoxelKey, std::pair<Point, size_t>, Hash> voxel_map;

    // Reserve memory for the voxel_map to minimize rehashing
    voxel_map.reserve(input_cloud.size());

    for (const auto &point : input_cloud)
    {
        VoxelKey key{static_cast<std::int32_t>(std::floor(point.x * inv_leaf_size_)),
                     static_cast<std::int32_t>(std::floor(point.y * inv_leaf_size_)),
                     static_cast<std::int32_t>(std::floor(point.z * inv_leaf_size_))};

        auto &[sum, count] = voxel_map[key];

        sum.x += point.x;
        sum.y += point.y;
        sum.z += point.z;

        ++count;
    }

    std::vector<Point> downsampled_cloud;
    downsampled_cloud.reserve(voxel_map.size());

    for (const auto &[key, value] : voxel_map)
    {
        const auto &[sum, count] = value;
        downsampled_cloud.push_back({sum.x / count, sum.y / count, sum.z / count});
    }

    return downsampled_cloud;
}

} // namespace downsampling
