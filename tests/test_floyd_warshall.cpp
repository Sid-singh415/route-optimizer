#include "floyd_warshall.h"
#include <gtest/gtest.h>
using namespace route;

TEST(FloydWarshall, EmptyGraphProducesEmptyMatrix) {
    Graph g(0);
    AllPairs ap = floydWarshall(g);
    EXPECT_EQ(ap.n, 0);
    EXPECT_TRUE(ap.dist.empty());
}

TEST(FloydWarshall, SingleNodeGraph) {
    Graph g(1);
    AllPairs ap = floydWarshall(g);
    ASSERT_EQ(ap.n, 1);
    EXPECT_DOUBLE_EQ(ap.dist[0][0], 0.0);
}

TEST(FloydWarshall, DiagonalIsAlwaysZero) {
    Graph g(4);
    g.addEdge(0, 1, 1.0);
    g.addEdge(1, 2, 1.0);
    AllPairs ap = floydWarshall(g);
    for (int i = 0; i < ap.n; ++i)
        EXPECT_DOUBLE_EQ(ap.dist[i][i], 0.0);
}

TEST(FloydWarshall, DirectEdgeMatchesWeight) {
    Graph g(2);
    g.addEdge(0, 1, 7.0, /*undirected=*/false);
    AllPairs ap = floydWarshall(g);
    EXPECT_DOUBLE_EQ(ap.dist[0][1], 7.0);
}

TEST(FloydWarshall, FindsShortestMultiHopPath) {
    // 0->1->2 costs 1+1=2, direct 0->2 costs 10. All-pairs should prefer the hop.
    Graph g(3);
    g.addEdge(0, 1, 1.0, false);
    g.addEdge(1, 2, 1.0, false);
    g.addEdge(0, 2, 10.0, false);
    AllPairs ap = floydWarshall(g);
    EXPECT_DOUBLE_EQ(ap.dist[0][2], 2.0);
}

TEST(FloydWarshall, DisconnectedNodesStayAtInfinity) {
    // Node 2 is isolated: no path to or from it.
    Graph g(3);
    g.addEdge(0, 1, 5.0);
    AllPairs ap = floydWarshall(g);
    EXPECT_EQ(ap.dist[0][2], INF);
    EXPECT_EQ(ap.dist[2][0], INF);
    EXPECT_EQ(ap.dist[1][2], INF);
    // but node 2 can still "reach" itself
    EXPECT_DOUBLE_EQ(ap.dist[2][2], 0.0);
}

TEST(FloydWarshall, UndirectedEdgeGivesSymmetricDistances) {
    Graph g(2);
    g.addEdge(0, 1, 4.0); // undirected by default
    AllPairs ap = floydWarshall(g);
    EXPECT_DOUBLE_EQ(ap.dist[0][1], ap.dist[1][0]);
    EXPECT_DOUBLE_EQ(ap.dist[0][1], 4.0);
}

TEST(FloydWarshall, ParallelEdgesKeepShortestWeight) {
    // Two edges added between the same pair (e.g. via two addEdge calls);
    // Floyd-Warshall's direct-edge seeding should keep the smaller weight.
    Graph g(2);
    g.addEdge(0, 1, 9.0, false);
    g.addEdge(0, 1, 2.0, false);
    AllPairs ap = floydWarshall(g);
    EXPECT_DOUBLE_EQ(ap.dist[0][1], 2.0);
}
