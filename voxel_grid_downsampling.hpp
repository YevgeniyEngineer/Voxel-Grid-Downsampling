#ifndef VOXEL_GRID_DOWNSAMPLING_HPP
#define VOXEL_GRID_DOWNSAMPLING_HPP

#include <cstdint>
#include <cstdlib>
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

struct Hash
{
    inline std::size_t operator()(const VoxelKey &key) const;
};

class VoxelGridDownSampler
{
  public:
    explicit VoxelGridDownSampler(float leaf_size_x, float leaf_size_y, float leaf_size_z);
    std::vector<Point> downsample(const std::vector<Point> &input_cloud, bool apply_averaging = false);

  private:
    float inv_leaf_size_x_;
    float inv_leaf_size_y_;
    float inv_leaf_size_z_;
};
} // namespace downsampling

#endif // VOXEL_GRID_DOWNSAMPLING_HPP