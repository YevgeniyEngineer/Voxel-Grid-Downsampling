cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(voxel_grid_downsampling)

add_library(${PROJECT_NAME} SHARED README.md
    ${CMAKE_CURRENT_SOURCE_DIR}/include/voxel_grid_downsampling.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(TEST_NAME test_${PROJECT_NAME})
add_executable(${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/test/test_voxel_grid_downsampling.cpp)
target_link_libraries(${TEST_NAME} ${PROJECT_NAME})