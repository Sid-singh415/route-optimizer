#include "dijkstra.h"
#include <gtest/gtest.h>
using namespace route;

TEST(Dijkstra, SimpleChain) {
    Graph g(3);
    g.addEdge(0, 1, 2.0, /*undirected=*/false);
    g.addEdge(1, 2, 3.0, /*undirected=*/false);
    auto sp = dijkstra(g, 0);
    EXPECT_DOUBLE_EQ(sp.dist[0], 0.0);
    EXPECT_DOUBLE_EQ(sp.dist[1], 2.0);
    EXPECT_DOUBLE_EQ(sp.dist[2], 5.0);
}

TEST(Dijkstra, PicksShorterOfTwoPaths) {
    // 0 -> 1 -> 3 costs 1+1=2; 0 -> 2 -> 3 costs 5+5=10. Shortest should win.
    Graph g(4);
    g.addEdge(0, 1, 1.0, false);
    g.addEdge(1, 3, 1.0, false);
    g.addEdge(0, 2, 5.0, false);
    g.addEdge(2, 3, 5.0, false);
    auto sp = dijkstra(g, 0);
    EXPECT_DOUBLE_EQ(sp.dist[3], 2.0);
    EXPECT_EQ(sp.parent[3], 1);
    EXPECT_EQ(sp.parent[1], 0);
}

TEST(Dijkstra, SingleNodeGraph) {
    Graph g(1);
    auto sp = dijkstra(g, 0);
    ASSERT_EQ(sp.dist.size(), 1u);
    EXPECT_DOUBLE_EQ(sp.dist[0], 0.0);
    EXPECT_EQ(sp.parent[0], -1);
}

TEST(Dijkstra, DisconnectedNodeStaysUnreachable) {
    // Node 2 has no edges at all, so it should remain at INF with no parent.
    Graph g(3);
    g.addEdge(0, 1, 4.0, false);
    auto sp = dijkstra(g, 0);
    EXPECT_DOUBLE_EQ(sp.dist[1], 4.0);
    EXPECT_EQ(sp.dist[2], INF);
    EXPECT_EQ(sp.parent[2], -1);
}

TEST(Dijkstra, UndirectedEdgeIsTraversableBothWays) {
    Graph g(2);
    g.addEdge(0, 1, 3.0); // undirected by default
    auto forward = dijkstra(g, 0);
    auto backward = dijkstra(g, 1);
    EXPECT_DOUBLE_EQ(forward.dist[1], 3.0);
    EXPECT_DOUBLE_EQ(backward.dist[0], 3.0);
}

TEST(Dijkstra, ReconstructPathReturnsFullRoute) {
    Graph g(4);
    g.addEdge(0, 1, 1.0, false);
    g.addEdge(1, 2, 1.0, false);
    g.addEdge(2, 3, 1.0, false);
    auto sp = dijkstra(g, 0);
    auto path = reconstructPath(sp, 0, 3);
    EXPECT_EQ(path, (std::vector<int>{0, 1, 2, 3}));
}

TEST(Dijkstra, ReconstructPathToUnreachableNodeIsEmpty) {
    Graph g(3);
    g.addEdge(0, 1, 1.0, false);
    // node 2 is unreachable from 0
    auto sp = dijkstra(g, 0);
    auto path = reconstructPath(sp, 0, 2);
    EXPECT_TRUE(path.empty());
}

TEST(Dijkstra, ReconstructPathSourceToItselfIsSingleNode) {
    Graph g(2);
    g.addEdge(0, 1, 1.0, false);
    auto sp = dijkstra(g, 0);
    auto path = reconstructPath(sp, 0, 0);
    EXPECT_EQ(path, (std::vector<int>{0}));
}
