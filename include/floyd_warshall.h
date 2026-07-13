#ifndef ROUTE_FLOYD_WARSHALL_H
#define ROUTE_FLOYD_WARSHALL_H

#include "graph.h"
#include <vector>

namespace route {

// Dense all-pairs shortest-path result.
struct AllPairs {
    std::vector<std::vector<double>> dist; // dist[i][j] = shortest i->j, INF if none
    int n = 0;
};

// Compute all-pairs shortest paths with Floyd-Warshall in O(V^3).
// Useful when you need many-to-many distances (e.g. comparing routes across
// a fleet) and V is small enough that V^3 beats running Dijkstra V times.
AllPairs floydWarshall(const Graph& g);

} // namespace route

#endif // ROUTE_FLOYD_WARSHALL_H
