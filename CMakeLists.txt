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
add_library(voxel_grid_downsampling::voxel_grid_downsampling ALIAS ${PROJECT_NAME})

set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Default install prefix")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}" CACHE PATH "Installation directory for CMake files")

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION "${INSTALL_INCLUDE_DIR}"
)

install(FILES voxel_grid_downsampling.hpp DESTINATION "${INSTALL_INCLUDE_DIR}/${PROJECT_NAME}")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

export(EXPORT ${PROJECT_NAME}Targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake"
    NAMESPACE ${PROJECT_NAME}::
)

configure_file(cmake/${PROJECT_NAME}Config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake"
    COPYONLY
)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION "${INSTALL_CMAKE_DIR}"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}"
)

# Test library
set(TEST_NAME test_${PROJECT_NAME})
add_executable(${TEST_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/test_voxel_grid_downsampling.cpp)
target_link_libraries(${TEST_NAME}
    PRIVATE
    voxel_grid_downsampling::voxel_grid_downsampling
)

# Optional Python Binding
if(ADD_PYTHON_BINDING)
    find_package(Python COMPONENTS Interpreter Development REQUIRED)
    find_package(pybind11 CONFIG REQUIRED)

    set(PYBIND_BINDING_NAME voxel_grid_downsampling_binding)
    pybind11_add_module(${PYBIND_BINDING_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/voxel_grid_downsampling_binding.cpp)
    target_link_libraries(${PYBIND_BINDING_NAME}
        PRIVATE
        voxel_grid_downsampling::voxel_grid_downsampling
        pybind11::headers
    )
    set_target_properties(${PYBIND_BINDING_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION ON
        VISIBILITY_INLINES_HIDDEN ON
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    install(FILES voxel_grid_downsampling_binding.pyi DESTINATION "${INSTALL_INCLUDE_DIR}/${PYBIND_BINDING_NAME}")
    install(TARGETS ${PYBIND_BINDING_NAME}
        LIBRARY DESTINATION lib
    )
endif()