# Multi-Stop Delivery Route Optimizer

A C++ tool that models city locations as a weighted graph and computes efficient
multi-stop delivery routes. Built to demonstrate graph algorithms, heuristics,
and the engineering around them (tests, benchmarks, clean build).

## Algorithms
- **Dijkstra** (min-heap) — point-to-point shortest paths
- **Floyd-Warshall** — all-pairs shortest paths for many-to-many route comparison
- **Nearest-neighbour** — greedy tour construction (baseline)
- **2-opt** — local search that refines the greedy tour

## Build
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/route_optimizer
```

## Test
```bash
cmake --build build --target unit_tests
ctest --test-dir build
```

## Status
Scaffold in place; algorithms implemented incrementally (see commit history).

## Design notes / tradeoffs
_(to be written as algorithms land — Floyd-Warshall vs repeated Dijkstra,
NN vs exact, why 2-opt)_
