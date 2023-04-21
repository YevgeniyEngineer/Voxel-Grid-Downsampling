#include "voxel_grid_downsampling.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <sstream>
#include <string>
#include <vector>

namespace py = pybind11;

PYBIND11_MODULE(voxel_grid_downsampling_binding, m)
{
    using namespace downsampling;

    m.doc() = "Voxel Grid Downsampling";

    py::class_<Point>(m, "Point")
        .def(py::init<>())
        .def(py::init<float, float, float>())
        .def_readwrite("x", &Point::x)
        .def_readwrite("y", &Point::y)
        .def_readwrite("z", &Point::z)
        .def("__str__",
             [](const Point &self) -> std::string {
                 std::stringstream ss;
                 ss << "(" << self.x << ", " << self.y << ", " << self.z << ")";
                 return ss.str();
             })
        .def("__repr__", [](const Point &self) -> std::string {
            std::stringstream ss;
            ss << "(" << self.x << ", " << self.y << ", " << self.z << ")";
            return ss.str();
        });

    py::class_<VoxelGridDownSampler>(m, "VoxelGridDownSampler")
        .def(py::init<float, float, float>())
        .def("downsample", &VoxelGridDownSampler::downsample);
}