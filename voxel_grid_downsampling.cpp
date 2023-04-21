#include "voxel_grid_downsampling.hpp"
#include "xxhash.h"

#include <cmath>
#include <stdexcept>
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

VoxelGridDownsampling::VoxelGridDownsampling(float leaf_size_x, float leaf_size_y, float leaf_size_z)
{
    if (leaf_size_x < 1e-6)
    {
        throw std::runtime_error("leaf size x less than minimum threshold!");
    }
    if (leaf_size_y < 1e-6)
    {
        throw std::runtime_error("leaf size y less than minimum threshold!");
    }
    if (leaf_size_z < 1e-6)
    {
        throw std::runtime_error("leaf size z less than minimum threshold!");
    }
    inv_leaf_size_x_ = 1.f / leaf_size_x;
    inv_leaf_size_y_ = 1.f / leaf_size_y;
    inv_leaf_size_z_ = 1.f / leaf_size_z;
}

std::vector<Point> VoxelGridDownsampling::downsample(const std::vector<Point> &input_cloud, bool apply_averaging)
{
    std::vector<Point> downsampled_cloud;

    if (apply_averaging)
    {
        std::unordered_map<VoxelKey, std::pair<Point, size_t>, Hash> voxel_map;
        voxel_map.reserve(input_cloud.size());

        for (const auto &point : input_cloud)
        {
            VoxelKey key{static_cast<std::int32_t>(std::floor(point.x * inv_leaf_size_x_)),
                         static_cast<std::int32_t>(std::floor(point.y * inv_leaf_size_y_)),
                         static_cast<std::int32_t>(std::floor(point.z * inv_leaf_size_z_))};

            auto &[sum, count] = voxel_map[key];

            sum.x += point.x;
            sum.y += point.y;
            sum.z += point.z;

            ++count;
        }

        downsampled_cloud.reserve(voxel_map.size());
        for (const auto &[key, value] : voxel_map)
        {
            const auto &[sum, count] = value;
            downsampled_cloud.push_back({sum.x / count, sum.y / count, sum.z / count});
        }
    }
    else
    {
        std::unordered_map<VoxelKey, Point, Hash> voxel_map;
        voxel_map.reserve(input_cloud.size());

        for (const auto &point : input_cloud)
        {
            VoxelKey key{static_cast<std::int32_t>(std::floor(point.x * inv_leaf_size_x_)),
                         static_cast<std::int32_t>(std::floor(point.y * inv_leaf_size_y_)),
                         static_cast<std::int32_t>(std::floor(point.z * inv_leaf_size_z_))};

            // Insert the point into the voxel_map if the voxel is empty.
            // If the voxel already contains a point, the insert operation does nothing.
            voxel_map.insert({key, point});
        }

        downsampled_cloud.reserve(voxel_map.size());
        for (const auto &[key, point] : voxel_map)
        {
            downsampled_cloud.push_back(point);
        }
    }

    return downsampled_cloud;
}

} // namespace downsampling
