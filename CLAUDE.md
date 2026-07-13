# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

A C++17 tool that models city locations as a weighted graph and computes efficient multi-stop
delivery routes. It's a learning/portfolio project demonstrating graph algorithms and heuristics
(Dijkstra, Floyd-Warshall, nearest-neighbour, 2-opt), plus the engineering around them (tests,
benchmarks, clean CMake build).

## Build & run

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

Run a single test binary directly for more control (e.g. `--gtest_filter`):

```bash
./build/unit_tests --gtest_filter=Dijkstra.*
```

Test files live in `tests/` and must be added to the `unit_tests` target sources in
`CMakeLists.txt` — they aren't auto-globbed.

## Architecture

- **`route_core`** (`src/*.cpp` + `include/*.h`, excluding `main.cpp`) is a library with no
  `main()` — all algorithm logic lives here. `route_optimizer` (`src/main.cpp`) and `benchmark`
  (`benchmark/benchmark.cpp`) both link against it. Add new algorithms as a `.cpp`/`.h` pair
  registered in `route_core`'s source list in `CMakeLists.txt`, not directly in an executable.
- **Layered dependency chain**: `graph.h` -> `dijkstra.h` / `floyd_warshall.h` -> `nearest_neighbour.h`
  -> `two_opt.h`. Each stage builds on the previous one's output type:
  - `Graph` — adjacency-list weighted graph, integer node IDs `[0, n)`, with optional
    name<->id lookup for future CSV/NL input.
  - `dijkstra()` — single-source shortest paths (binary heap), for point-to-point queries.
  - `floydWarshall()` — dense all-pairs shortest paths (`AllPairs`, O(V^3)), used when many-to-many
    distances are needed and V is small.
  - `nearestNeighbour()` — greedy tour construction over a subset of stops, operating on the
    `AllPairs` matrix (i.e. the metric closure of the graph, not raw edges).
  - `twoOpt()` — local search that improves a `Tour` by reversing segments while it reduces length.
- All public types/functions live in the `route` namespace.
- Everything is in the `route` namespace under `include/`; headers are self-contained (include
  what they use, e.g. `two_opt.h` includes `nearest_neighbour.h` for the `Tour` type).
- `data/` and `src/output/` exist but are currently empty — reserved for future CSV I/O.
- Tests use GoogleTest, fetched via CMake `FetchContent` (no system install needed); test
  discovery is via `gtest_discover_tests`.

## Status

Scaffold in place; algorithms are implemented incrementally (see commit history for the order:
Dijkstra -> Floyd-Warshall -> nearest-neighbour/2-opt). `benchmark/benchmark.cpp` and
`tests/test_tour.cpp` are currently placeholders/TODOs, not finished suites.
