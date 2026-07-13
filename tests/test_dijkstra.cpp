#include "dijkstra.h"
#include <gtest/gtest.h>
using namespace route;

// Placeholder — you'll flesh these out on Day 5.
// Once dijkstra() is implemented, flip DISABLED_ off to activate.
TEST(Dijkstra, DISABLED_SimpleChain) {
    Graph g(3);
    g.addEdge(0, 1, 2.0);
    g.addEdge(1, 2, 3.0);
    auto sp = dijkstra(g, 0);
    EXPECT_DOUBLE_EQ(sp.dist[2], 5.0);
}
