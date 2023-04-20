#include "voxel_grid_downsampling.hpp"

#include <cmath>
#include <functional>
#include <unordered_map>
#include <utility>

namespace downsampling
{
inline bool VoxelKey::operator==(const VoxelKey &other) const
{
    return (x == other.x) && (y == other.y) && (z == other.z);
}

inline std::size_t Hash::wangHash(const VoxelKey &key) const
{
    auto wangMix = [](std::size_t &h) {
        h = (h ^ 61) ^ (h >> 16);
        h = h + (h << 3);
        h = h ^ (h >> 4);
        h = h * 0x27d4eb2d;
        h = h ^ (h >> 15);
    };

    std::size_t h = static_cast<std::size_t>(key.x);
    wangMix(h);
    h ^= static_cast<std::size_t>(key.y);
    wangMix(h);
    h ^= static_cast<std::size_t>(key.z);
    wangMix(h);

    return h;
}

inline std::size_t Hash::murmurHash3(const VoxelKey &key) const
{
    auto murmur3Finalizer = [](std::uint32_t h) {
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;
        return static_cast<std::size_t>(h);
    };

    std::uint32_t h = 0;
    h ^= static_cast<std::uint32_t>(key.x) * 0xcc9e2d51;
    h = (h << 15) | (h >> (32 - 15));
    h *= 0x1b873593;

    h ^= static_cast<std::uint32_t>(key.y) * 0xcc9e2d51;
    h = (h << 15) | (h >> (32 - 15));
    h *= 0x1b873593;

    h ^= static_cast<std::uint32_t>(key.z) * 0xcc9e2d51;
    h = (h << 15) | (h >> (32 - 15));
    h *= 0x1b873593;

    return murmur3Finalizer(h);
}

inline std::size_t Hash::fnv1aHash(const VoxelKey &key) const
{
    constexpr std::size_t FNV_OFFSET_BASIS = 0xcbf29ce484222325;
    constexpr std::size_t FNV_PRIME = 0x100000001b3;

    std::size_t h = FNV_OFFSET_BASIS;
    h = (h ^ static_cast<std::size_t>(key.x)) * FNV_PRIME;
    h = (h ^ static_cast<std::size_t>(key.y)) * FNV_PRIME;
    h = (h ^ static_cast<std::size_t>(key.z)) * FNV_PRIME;

    return h;
}

inline std::size_t Hash::simpleHash(const VoxelKey &key) const
{
    std::size_t hx = std::hash<decltype(key.x)>{}(key.x);
    std::size_t hy = std::hash<decltype(key.y)>{}(key.y);
    std::size_t hz = std::hash<decltype(key.z)>{}(key.z);
    return (hx ^ (hy << 1)) ^ hz;
}

inline std::size_t Hash::operator()(const VoxelKey &key) const
{
    if constexpr (HASH_METHOD == HashMethod::WANG)
    {
        return wangHash(key);
    }
    else if constexpr (HASH_METHOD == HashMethod::MURMUR)
    {
        return murmurHash3(key);
    }
    else if constexpr (HASH_METHOD == HashMethod::FNV1A)
    {
        return fnv1aHash(key);
    }
    else
    {
        return simpleHash(key);
    }
}

VoxelGridDownsampling::VoxelGridDownsampling(float leaf_size) : leaf_size_(leaf_size), inv_leaf_size_(1.0f / leaf_size_)
{
}

std::vector<Point> VoxelGridDownsampling::downsample(const std::vector<Point> &input_cloud)
{
    std::unordered_map<VoxelKey, std::pair<Point, size_t>, Hash> voxel_map;
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
