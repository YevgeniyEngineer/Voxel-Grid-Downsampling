# Voxel-Grid-Downsampling
Point cloud down-sampling based on Voxel Grid

![image1](https://github.com/YevgeniyEngineer/Voxel-Grid-Downsampling/tree/main/images/result.png)

## Optional Dependencies
If `ADD_PYTHON_BINDING` is set to `TRUE` in `CMakeLists.txt`, then the build will generate Python Binding.

Bindings are generated using PyBind11:
`sudo apt update && sudo apt install python3-dev python3-pybind11`

## Examples
See `test_voxel_grid_downsampling.cpp` and `test_voxel_grid_downsampling.py` for usage.

## Build
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Install
```bash
sudo make install
```

## Use Externally
```cmake
find_package(voxel_grid_downsampling REQUIRED)

target_link_libraries(your_executable_name PRIVATE 
    voxel_grid_downsampling::voxel_grid_downsampling
)
```