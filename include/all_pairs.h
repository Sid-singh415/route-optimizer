#ifndef ROUTE_ALL_PAIRS_H
#define ROUTE_ALL_PAIRS_H

#include "floyd_warshall.h"

namespace route {

// Computes all-pairs shortest paths, picking whichever strategy is
// asymptotically cheaper for this specific graph's V (nodes) and E (edges):
//   - Floyd-Warshall:        O(V^3), independent of E.
//   - V x Dijkstra (heap):   O(V * (E + V) log V).
// Sparse graphs (E close to V) favor repeated Dijkstra; dense graphs
// (E close to V^2) favor Floyd-Warshall. The crossover is computed from the
// actual edge count rather than a fixed density cutoff.
AllPairs allPairsShortestPaths(const Graph& g);

} // namespace route

#endif // ROUTE_ALL_PAIRS_H
