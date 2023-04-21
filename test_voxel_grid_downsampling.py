import random
import sys
import time

sys.path.append("./build")

from voxel_grid_downsampling_binding import Point, VoxelGridDownSampler

NUM_PTS = 100_000

# Create a list of random points
input_cloud = []
for _ in range(NUM_PTS):
    x = random.uniform(-100.0, 100.0)
    y = random.uniform(-100.0, 100.0)
    z = random.uniform(-100.0, 100.0)
    input_cloud.append(Point(x, y, z))

# Create a new instance of the VoxelGridDownSampler class
leaf_size_x = 10.0
leaf_size_y = 10.0
leaf_size_z = 10.0
downsampler = VoxelGridDownSampler(leaf_size_x, leaf_size_y, leaf_size_z)

# Call the downsample method with the input cloud
t1 = time.perf_counter()
output_cloud = downsampler.downsample(input_cloud, False)
t2 = time.perf_counter()

# Print the size of the output cloud
print("Original number of points: ", len(input_cloud))
print("Number of points after down sampling: ", len(output_cloud))
print("Elapsed time (s): ", (t2 - t1))
