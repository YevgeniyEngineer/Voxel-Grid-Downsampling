#ifndef VOXEL_GRID_DOWNSAMPLING_HPP
#define VOXEL_GRID_DOWNSAMPLING_HPP

#include <cstdint>
#include <vector>

namespace downsampling
{
struct Point
{
    float x, y, z;
};

struct VoxelKey
{
    std::int32_t x, y, z;
    bool operator==(const VoxelKey &other) const;
};

enum class HashMethod : std::uint8_t
{
    SIMPLE,
    WANG,
    MURMUR,
    FNV1A
};

constexpr auto HASH_METHOD = HashMethod::WANG;

struct Hash
{
    inline std::size_t simpleHash(const VoxelKey &key) const;
    inline std::size_t wangHash(const VoxelKey &key) const;
    inline std::size_t murmurHash3(const VoxelKey &key) const;
    inline std::size_t fnv1aHash(const VoxelKey &key) const;
    inline std::size_t operator()(const VoxelKey &key) const;
};

class VoxelGridDownsampling
{
  public:
    VoxelGridDownsampling(float leaf_size);
    std::vector<Point> downsample(const std::vector<Point> &input_cloud);

  private:
    float leaf_size_;
    float inv_leaf_size_;
};

} // namespace downsampling

#endif // VOXEL_GRID_DOWNSAMPLING_HPP