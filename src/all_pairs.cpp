#include "all_pairs.h"
#include "dijkstra.h"
#include <cmath>

namespace route {

static AllPairs dijkstraAllPairs(const Graph& g) {
    int n = g.numNodes();
    AllPairs ap;
    ap.n = n;
    ap.dist.assign(n, std::vector<double>(n, INF));
    for (int src = 0; src < n; ++src) {
        ShortestPaths sp = dijkstra(g, src);
        ap.dist[src] = std::move(sp.dist);
    }
    return ap;
}

AllPairs allPairsShortestPaths(const Graph& g) {
    long long v = g.numNodes();
    if (v == 0) return floydWarshall(g);

    long long e = 0;
    for (int u = 0; u < v; ++u) e += static_cast<long long>(g.neighbours(u).size());

    double logV = std::log2(static_cast<double>(v) + 1.0);
    double floydWarshallCost = static_cast<double>(v) * v * v;
    double dijkstraCost = static_cast<double>(v) * (e + v) * logV;

    return (dijkstraCost < floydWarshallCost) ? dijkstraAllPairs(g) : floydWarshall(g);
}

} // namespace route
