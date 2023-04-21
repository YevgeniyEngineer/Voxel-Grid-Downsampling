import random
import sys
import time
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D

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

# Plot clouds
input_cloud_list = []
for point in input_cloud:
    input_cloud_list.append((point.x, point.y, point.z))

output_cloud_list = []
for point in output_cloud:
    output_cloud_list.append((point.x, point.y, point.z))

input_cloud_list = np.asarray(input_cloud_list)
output_cloud_list = np.asarray(output_cloud_list)

# Create a new figure
fig = plt.figure()

# Create the first subplot
ax1 = fig.add_subplot(121, projection="3d")
ax1.scatter(
    input_cloud_list[:, 0],
    input_cloud_list[:, 1],
    input_cloud_list[:, 2],
    c="r",
    marker="o",
)
ax1.set_title(f"Point Cloud {len(input_cloud_list)} Points")
ax1.set_xlabel("X")
ax1.set_ylabel("Y")
ax1.set_zlabel("Z")

# Create the second subplot
ax2 = fig.add_subplot(122, projection="3d")
ax2.scatter(
    output_cloud_list[:, 0],
    output_cloud_list[:, 1],
    output_cloud_list[:, 2],
    c="b",
    marker="^",
)
ax2.set_title(f"Downsampled Point Cloud {len(output_cloud_list)} Points")
ax2.set_xlabel("X")
ax2.set_ylabel("Y")
ax2.set_zlabel("Z")

# Show the plot
plt.show()
