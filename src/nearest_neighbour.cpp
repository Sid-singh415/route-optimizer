#include "nearest_neighbour.h"
#include <vector>

namespace route {

// Total length of a given tour order under the all-pairs matrix.
// Walks consecutive stops and adds each leg, then closes back to the start.
double tourLength(const AllPairs& ap, const std::vector<int>& order) {
    double total = 0.0;
    for (size_t i = 0; i + 1 < order.size(); ++i)
        total += ap.dist[order[i]][order[i + 1]];
    // close the loop: last stop back to the first
    if (order.size() > 1)
        total += ap.dist[order.back()][order.front()];
    return total;
}

Tour nearestNeighbour(const AllPairs& ap, const std::vector<int>& stops, int start_index) {
    Tour tour;
    int k = stops.size();
    if (k == 0) return tour;

    // track which stops (by their index in `stops`) we've visited
    std::vector<bool> visited(k, false);

    // 1. start at stops[start_index]
    int current = start_index;          // index into `stops`
    visited[current] = true;            // blank 1: mark start visited
    tour.order.push_back(stops[current]);

    // 2. visit the remaining k-1 stops, each time hopping to the nearest unvisited
    for (int step = 0; step < k - 1; ++step) {
        int best = -1;
        double bestDist = INF;

        // scan all stops, find the nearest UNVISITED one to `current`
        for (int j = 0; j < k; ++j) {
            if (visited[j]) continue;         // blank 2: skip if already visited

            double d = ap.dist[stops[current]][stops[j]];
            if (d < bestDist) {
                bestDist = d;
                best = j;            // blank 3: remember which stop is closest
            }
        }

        // 3. go to `best`
        visited[best] = true;
        tour.order.push_back(stops[best]);
        tour.length += bestDist;            // blank 4: add the leg we just travelled
        current = best;
    }

    // 4. close the loop: return to the starting stop
    tour.length += ap.dist[stops[current]][stops[start_index]];

    return tour;
}

} // namespace route