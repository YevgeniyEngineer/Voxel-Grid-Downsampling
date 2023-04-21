cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(voxel_grid_downsampling)
set(ADD_PYTHON_BINDING TRUE)

# Create dynamically linked (shared) library
add_subdirectory(xxHash)
add_library(${PROJECT_NAME} SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/voxel_grid_downsampling.cpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/xxHash
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    xxhash
)
add_library(voxel_grid_downsampling::lib ALIAS ${PROJECT_NAME})

# Test library
set(TEST_NAME test_${PROJECT_NAME})
add_executable(${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/test_voxel_grid_downsampling.cpp)
target_link_libraries(${TEST_NAME}
    PRIVATE
    voxel_grid_downsampling::lib
)

# Optional Python Binding
if(ADD_PYTHON_BINDING)
    find_package(Python COMPONENTS Interpreter Development REQUIRED)
    find_package(pybind11 CONFIG REQUIRED)

    set(PYBIND_BINDING_NAME voxel_grid_downsampling_binding)
    pybind11_add_module(${PYBIND_BINDING_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/voxel_grid_downsampling_binding.cpp)
    target_link_libraries(${PYBIND_BINDING_NAME}
        PRIVATE
        voxel_grid_downsampling::lib
        pybind11::headers
    )
    set_target_properties(${PYBIND_BINDING_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION ON
        VISIBILITY_INLINES_HIDDEN ON
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
endif()