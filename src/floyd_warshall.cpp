#include "floyd_warshall.h"
#include <algorithm>

namespace route {

AllPairs floydWarshall(const Graph& g) {
    int n = g.numNodes();
    AllPairs ap;
    ap.n = n;

    ap.dist.assign(n, std::vector<double>(n, INF));

    for (int i = 0; i < n; ++i)
        ap.dist[i][i] = 0;

    for (int u = 0; u < n; ++u) {
        for (const Edge& e : g.neighbours(u)) {
            ap.dist[u][e.to] = std::min(ap.dist[u][e.to], e.weight);
        }
    }

    for (int h = 0; h < n; ++h) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (ap.dist[u][h] != INF && ap.dist[h][v] != INF) {
                    ap.dist[u][v] = std::min(ap.dist[u][v], ap.dist[u][h] + ap.dist[h][v]);
                }
            }
        }
    }

    return ap;
}

} // namespace route