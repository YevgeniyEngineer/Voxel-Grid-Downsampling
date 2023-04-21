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

struct Hash
{
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