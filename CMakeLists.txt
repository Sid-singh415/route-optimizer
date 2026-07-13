cmake_minimum_required(VERSION 3.14)
project(route_optimizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# --- JSON (nlohmann/json, FetchContent, same pattern as googletest below) ---
include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# --- Core library (algorithms live here, no main()) ---
add_library(route_core
  src/graph.cpp
  src/dijkstra.cpp
  src/floyd_warshall.cpp
  src/all_pairs.cpp
  src/nearest_neighbour.cpp
  src/two_opt.cpp
  src/geo_utils.cpp
  src/osm_loader.cpp
  src/delivery_points.cpp
  src/snap.cpp
  src/route_json.cpp
)
target_include_directories(route_core PUBLIC include)
target_link_libraries(route_core PUBLIC nlohmann_json::nlohmann_json)

# --- Main executable ---
add_executable(route_optimizer src/main.cpp)
target_link_libraries(route_optimizer PRIVATE route_core)

# --- Benchmark executable ---
add_executable(benchmark benchmark/benchmark.cpp)
target_link_libraries(benchmark PRIVATE route_core)

# --- Chandigarh real-road-network demo ---
add_executable(chandigarh_demo src/chandigarh_demo.cpp)
target_link_libraries(chandigarh_demo PRIVATE route_core)

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
  tests/test_all_pairs.cpp
  tests/test_tour.cpp
  tests/test_osm_loader.cpp
  tests/test_snap.cpp
  tests/test_route_json.cpp
)
target_link_libraries(unit_tests PRIVATE route_core GTest::gtest_main)
target_compile_definitions(unit_tests PRIVATE
  TEST_FIXTURES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures")

include(GoogleTest)
gtest_discover_tests(unit_tests)
