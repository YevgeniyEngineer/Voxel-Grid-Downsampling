cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(xxhash)

set(XXHASH_SOURCES
    xxhash.c)

set(XXHASH_HEADERS
    xxhash.h)

# Check if the target platform is x86 or x86_64
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|x86")
    list(APPEND XXHASH_SOURCES xxh_x86dispatch.c)
    list(APPEND XXHASH_HEADERS xxh_x86dispatch.h)
endif()

add_library(xxhash STATIC ${XXHASH_SOURCES} ${XXHASH_HEADERS})

target_include_directories(xxhash PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})