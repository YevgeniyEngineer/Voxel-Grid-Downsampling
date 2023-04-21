from typing import List

class Point:
    def __init__(self, x: float, y: float, z: float) -> None: ...

class VoxelGridDownSampler:
    def __init__(
        self, leaf_size_x: float, leaf_size_y: float, leaf_size_z: float
    ) -> None: ...
    def downsample(input_cloud: List[Point], apply_averaging: bool) -> List[Point]: ...
