cmake_minimum_required(VERSION 3.14)
project(route_optimizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# --- Core library (algorithms live here, no main()) ---
add_library(route_core
  src/graph.cpp
  src/dijkstra.cpp
  src/floyd_warshall.cpp
  src/nearest_neighbour.cpp
  src/two_opt.cpp
)
target_include_directories(route_core PUBLIC include)

# --- Main executable ---
add_executable(route_optimizer src/main.cpp)
target_link_libraries(route_optimizer PRIVATE route_core)

# --- Benchmark executable ---
add_executable(benchmark benchmark/benchmark.cpp)
target_link_libraries(benchmark PRIVATE route_core)

# --- Tests (GoogleTest via FetchContent) ---
enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
  tests/test_dijkstra.cpp
  tests/test_floyd_warshall.cpp
  tests/test_tour.cpp
)
target_link_libraries(unit_tests PRIVATE route_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
