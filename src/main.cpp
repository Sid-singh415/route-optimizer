#include "graph.h"
#include "dijkstra.h"
#include "floyd_warshall.h"
#include <iostream>
#include <iomanip>

int main() {
    using namespace route;

    // Tiny sample graph:
    //   0 --2-- 1 --1-- 2
    //   |               |
    //   +------4--------+   (0-3 weight 4, 2-3 weight 1)
    Graph g(4);
    g.addEdge(0, 1, 2.0);
    g.addEdge(1, 2, 1.0);
    g.addEdge(0, 3, 4.0);
    g.addEdge(2, 3, 1.0);

    // --- Dijkstra from node 0 ---
    ShortestPaths sp = dijkstra(g, 0);
    std::cout << "Dijkstra: shortest distances from node 0\n";
    for (int v = 0; v < g.numNodes(); ++v) {
        std::cout << "  0 -> " << v << " = ";
        if (sp.dist[v] == INF) std::cout << "unreachable";
        else                   std::cout << sp.dist[v];
        std::cout << "\n";
    }

    // --- Floyd-Warshall all-pairs ---
    AllPairs ap = floydWarshall(g);
    std::cout << "\nFloyd-Warshall: all-pairs distance matrix\n     ";
    for (int j = 0; j < ap.n; ++j) std::cout << std::setw(5) << j;
    std::cout << "\n";
    for (int i = 0; i < ap.n; ++i) {
        std::cout << std::setw(3) << i << ": ";
        for (int j = 0; j < ap.n; ++j) {
            if (ap.dist[i][j] == INF) std::cout << std::setw(5) << "INF";
            else                      std::cout << std::setw(5) << ap.dist[i][j];
        }
        std::cout << "\n";
    }

    return 0;
}