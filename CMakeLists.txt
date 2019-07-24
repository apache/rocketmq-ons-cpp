cmake_minimum_required(VERSION 3.0)
set(CLIENT_VERSION 2.0.0)
set(LIBRARY_NAME onsclient4cpp)
set(PROJECT_NAME rocketmq-ons-cpp)
project(${PROJECT_NAME} VERSION ${CLIENT_VERSION}
        LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

include(CTest)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_BENCHMARK "Build benchmark tests" OFF)
option(BUILD_DEMOS "Build demos" ON)

include_directories(graalvm_artifacts
        src/main/c/native
        src/main/cpp/include
        src/main/cpp/sdk/common)

find_library(ROCKETMQ_CLIENT_CORE
        NAMES rocketmq_client_core
        HINTS ${CMAKE_SOURCE_DIR}/build)

if (${ROCKETMQ_CLIENT_CORE-NOTFOUNT})
    message("find_library for rocketmq_client_core failed")
endif ()

add_subdirectory(src/main/cpp)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(3rd_party/google_test)
    add_subdirectory(src/test/cpp)
endif ()

if (BUILD_DEMOS)
    add_subdirectory(src/main/cpp/demos)
endif ()

if (BUILD_BENCHMARK)
    set(BENCHMARK_ENABLE_TESTING OFF)
    add_subdirectory(3rd_party/benchmark)
    add_subdirectory(src/main/cpp/benchmark)
endif ()

