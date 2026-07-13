#include "graph.h"
#include "dijkstra.h"
#include <iostream>

// Minimal driver for now. Grows into CSV loading + full route reporting later.
int main() {
    using namespace route;

    // Tiny sample graph:
    //   0 --2-- 1 --1-- 2
    //   |               |
    //   4------- 3 ----- (0-3 weight 4, 2-3 weight 1)
    Graph g(4);
    g.addEdge(0, 1, 2.0);
    g.addEdge(1, 2, 1.0);
    g.addEdge(0, 3, 4.0);
    g.addEdge(2, 3, 1.0);

    ShortestPaths sp = dijkstra(g, 0);

    std::cout << "Shortest distances from node 0:\n";
    for (int v = 0; v < g.numNodes(); ++v) {
        std::cout << "  0 -> " << v << " = ";
        if (sp.dist[v] == INF) std::cout << "unreachable";
        else                   std::cout << sp.dist[v];
        std::cout << "\n";
    }

    std::cout << "\n(Distances will read as 'unreachable' until you implement dijkstra().)\n";
    return 0;
}
