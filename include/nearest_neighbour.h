#ifndef ROUTE_NEAREST_NEIGHBOUR_H
#define ROUTE_NEAREST_NEIGHBOUR_H

#include "floyd_warshall.h"
#include <vector>

namespace route {

// A tour over a subset of stops, plus its total length.
struct Tour {
    std::vector<int> order;   // sequence of stop node-ids, starting and ending at start
    double length = 0.0;
};

// Build a tour visiting all `stops` starting from stops[start_index], greedily
// hopping to the nearest unvisited stop. Distances come from a precomputed
// all-pairs matrix (so this works on the metric closure of the graph).
Tour nearestNeighbour(const AllPairs& ap, const std::vector<int>& stops, int start_index = 0);

// Total length of a given tour order under the all-pairs matrix (returns to start).
double tourLength(const AllPairs& ap, const std::vector<int>& order);

} // namespace route

#endif // ROUTE_NEAREST_NEIGHBOUR_H
