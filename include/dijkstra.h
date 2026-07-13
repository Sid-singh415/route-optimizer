#ifndef ROUTE_DIJKSTRA_H
#define ROUTE_DIJKSTRA_H

#include "graph.h"
#include <vector>

namespace route {

// Result of a single-source Dijkstra run.
struct ShortestPaths {
    std::vector<double> dist;   // dist[v] = shortest distance from source, INF if unreachable
    std::vector<int>    parent; // parent[v] = predecessor of v on shortest path, -1 if none
};

// Compute shortest paths from `source` to all nodes using Dijkstra's algorithm
// with a binary min-heap (std::priority_queue).
// Precondition: all edge weights are non-negative.
ShortestPaths dijkstra(const Graph& g, int source);

// Reconstruct the node sequence source -> target from a ShortestPaths result.
// Returns empty vector if target is unreachable.
std::vector<int> reconstructPath(const ShortestPaths& sp, int source, int target);

} // namespace route

#endif // ROUTE_DIJKSTRA_H
