#include "all_pairs.h"
#include "floyd_warshall.h"
#include <gtest/gtest.h>
using namespace route;

// allPairsShortestPaths() may internally choose Floyd-Warshall or repeated
// Dijkstra depending on graph density, but the result must match
// floydWarshall() exactly regardless of which path it takes.
static void expectSameDistances(const AllPairs& a, const AllPairs& b) {
    ASSERT_EQ(a.n, b.n);
    for (int i = 0; i < a.n; ++i)
        for (int j = 0; j < a.n; ++j)
            EXPECT_DOUBLE_EQ(a.dist[i][j], b.dist[i][j]) << "at (" << i << "," << j << ")";
}

TEST(AllPairs, EmptyGraph) {
    Graph g(0);
    AllPairs ap = allPairsShortestPaths(g);
    EXPECT_EQ(ap.n, 0);
    EXPECT_TRUE(ap.dist.empty());
}

TEST(AllPairs, SingleNodeGraph) {
    Graph g(1);
    AllPairs ap = allPairsShortestPaths(g);
    ASSERT_EQ(ap.n, 1);
    EXPECT_DOUBLE_EQ(ap.dist[0][0], 0.0);
}

TEST(AllPairs, SparseGraphMatchesFloydWarshall) {
    // A sparse chain (E ~ V): this is the shape where the dispatcher should
    // prefer repeated Dijkstra, but the answer must still agree with FW.
    Graph g(6);
    g.addEdge(0, 1, 7.0, false);
    g.addEdge(1, 2, 3.0, false);
    g.addEdge(2, 3, 4.0, false);
    g.addEdge(3, 4, 2.0, false);
    g.addEdge(4, 5, 6.0, false);

    AllPairs viaDispatcher = allPairsShortestPaths(g);
    AllPairs viaFW = floydWarshall(g);
    expectSameDistances(viaDispatcher, viaFW);
}

TEST(AllPairs, DenseGraphMatchesFloydWarshall) {
    // A dense, fully-connected graph (E ~ V^2): the dispatcher should prefer
    // Floyd-Warshall here, but again the result must agree either way.
    const int n = 6;
    Graph g(n);
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            g.addEdge(i, j, static_cast<double>(1 + (i * n + j) % 5), false);

    AllPairs viaDispatcher = allPairsShortestPaths(g);
    AllPairs viaFW = floydWarshall(g);
    expectSameDistances(viaDispatcher, viaFW);
}

TEST(AllPairs, DisconnectedNodesStayAtInfinity) {
    Graph g(3);
    g.addEdge(0, 1, 5.0);
    AllPairs ap = allPairsShortestPaths(g);
    EXPECT_EQ(ap.dist[0][2], INF);
    EXPECT_EQ(ap.dist[2][0], INF);
    EXPECT_DOUBLE_EQ(ap.dist[2][2], 0.0);
}
