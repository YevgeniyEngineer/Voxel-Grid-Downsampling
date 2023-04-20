#include "voxel_grid_downsampling.hpp"

using namespace downsampling;

#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

std::vector<Point> generateRandomPoints(std::size_t num_points, float min_value, float max_value)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min_value, max_value);

    std::vector<Point> points;
    points.reserve(num_points);

    for (std::size_t i = 0; i < num_points; ++i)
    {
        points.push_back({dis(gen), dis(gen), dis(gen)});
    }

    return points;
}

int main()
{
    // Test data
    const auto input_cloud = generateRandomPoints(100'000, -100.0f, 100.0f);

    float leaf_size = 10.0f;
    VoxelGridDownsampling downsampler(leaf_size);

    auto t1 = std::chrono::steady_clock::now();
    std::vector<Point> downsampled_cloud = downsampler.downsample(input_cloud);
    auto t2 = std::chrono::steady_clock::now();

    // for (const auto &point : downsampled_cloud)
    // {
    //     std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << ")\n";
    // }

    std::cout << "Number of downsampled points: " << downsampled_cloud.size() << std::endl;
    std::cout << "Downsampling time (s): " << (t2 - t1).count() / 1e9 << std::endl;

    return 0;
}
