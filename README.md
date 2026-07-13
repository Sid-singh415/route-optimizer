# Multi-Stop Delivery Route Optimizer

A C++ tool that models city locations as a weighted graph and computes efficient
multi-stop delivery routes. Built to demonstrate graph algorithms, heuristics,
and the engineering around them (tests, benchmarks, clean build).
![Uploading image.png…]()


## Algorithms
- **Dijkstra** (set) — point-to-point shortest paths
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

## Chandigarh real-road pipeline

Runs the same algorithms above on the real Chandigarh + Mohali road network
(loaded from OpenStreetMap), routing between a handful of real delivery
landmarks, and renders the result on an interactive map.

```bash
# 1. Fetch OSM data once (bounded to Chandigarh + Mohali IT City; re-run any
#    time you want fresher data)
./scripts/fetch_osm.sh

# 2. Build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# 3. Run -- writes route.json
./build/chandigarh_demo

# 4. View (fetch() against file:// is blocked by browser CORS, so serve it)
python3 -m http.server 8000
# then open http://localhost:8000/map.html
```

Delivery points live in `data/delivery_points.csv` (name, lat, lng) — edit
freely to route between different landmarks.

## Status
algorithms implemented incrementally (see commit history).

## Design notes / tradeoffs
- **Floyd-Warshall vs repeated Dijkstra**: Floyd-Warshall is O(V³), which is
  fine for the tiny toy graph in `main.cpp` but won't scale to a real road
  network (tens of thousands of nodes). The Chandigarh pipeline never runs
  Floyd-Warshall (or the `allPairsShortestPaths` dispatcher) on the full
  graph — it calls the existing `dijkstra()` once per delivery point
  (N runs, not V), each covering all N targets in one pass.
- **Every OSM node is a graph node**: rather than modeling only true
  intersections and storing a polyline per edge, every OSM way's shape point
  becomes its own `Graph` node, with one edge per consecutive pair. This
  means `reconstructPath()` — unmodified — returns the exact real-road
  polyline for free, with no separate polyline-reconstruction logic.
- **Local indices, not graph node ids**: `nearestNeighbour`/`twoOpt` index
  their `AllPairs` matrix using the values inside `stops`/`tour.order`
  directly. The Chandigarh pipeline builds a small N×N matrix (N = number of
  delivery points, not V = number of road nodes) and passes local indices
  `0..N-1` as `stops`, keeping a separate array mapping local index → real
  graph node id for everything else (Dijkstra calls, JSON output).
- **Highway filter includes `_link` roads**: excluding ramp/slip roads
  (`motorway_link`, `primary_link`, etc.) from the Overpass query silently
  disconnects the graph at interchanges — two real delivery points ended up
  in different connected components until these were included.
