#include "graph.h"
#include "floyd_warshall.h"
#include "nearest_neighbour.h"
#include "two_opt.h"
#include <iostream>

int main() {
    using namespace route;

    Graph g(6);
    g.addEdge(0, 1, 7.0);
    g.addEdge(0, 2, 9.0);
    g.addEdge(1, 2, 3.0);
    g.addEdge(1, 3, 5.0);
    g.addEdge(2, 3, 4.0);
    g.addEdge(2, 4, 2.0);
    g.addEdge(3, 5, 6.0);
    g.addEdge(4, 5, 8.0);

    AllPairs ap = floydWarshall(g);
    std::vector<int> stops = {0, 1, 3, 4, 5};

    Tour nn = nearestNeighbour(ap, stops, 0);
    std::cout << "Greedy (nearest-neighbour):  ";
    for (int x : nn.order) std::cout << x << " -> ";
    std::cout << nn.order.front() << "   length = " << nn.length << "\n";

    Tour opt = twoOpt(ap, nn);
    std::cout << "After 2-opt refinement:      ";
    for (int x : opt.order) std::cout << x << " -> ";
    std::cout << opt.order.front() << "   length = " << opt.length << "\n";

    double improvement = 100.0 * (nn.length - opt.length) / nn.length;
    std::cout << "Improvement: " << improvement << "%\n";

    return 0;
}