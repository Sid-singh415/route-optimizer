#include "osm_loader.h"
#include "geo_utils.h"
#include <gtest/gtest.h>
#include <algorithm>
using namespace route;

static std::string fixturePath(const std::string& name) {
    return std::string(TEST_FIXTURES_DIR) + "/" + name;
}

// Fixture layout (see tests/fixtures/tiny_osm.json):
//   way 100: nodes 1-2-3, plain two-way road
//   way 101: nodes 3-4-5, oneway=yes (forward only)
//   way 102: nodes 5-6,   oneway=-1  (reversed: 6 -> 5 only)
// Node 3 is shared between ways 100/101, node 5 between 101/102 --
// these should collapse to a single graph node id each (intersections).
// OSM ids 1..6 map to graph ids 0..5 in first-referenced order.

static bool hasEdgeTo(const Graph& g, int u, int v) {
    for (const Edge& e : g.neighbours(u))
        if (e.to == v) return true;
    return false;
}

TEST(OsmLoader, ProducesOneGraphNodePerDistinctOsmNode) {
    std::vector<NodeCoord> coords;
    Graph g = loadOsmGraph(fixturePath("tiny_osm.json"), coords);
    EXPECT_EQ(g.numNodes(), 6);
    EXPECT_EQ(coords.size(), 6u);
}

TEST(OsmLoader, SharedNodeCollapsesToSingleGraphId) {
    // OSM node 3 is graph id 2 (third node encountered by way 100). It must
    // have an edge *into* it from way 100 and an edge *out of* it from way
    // 101 on the SAME graph id -- proving the two ways connect at one node.
    std::vector<NodeCoord> coords;
    Graph g = loadOsmGraph(fixturePath("tiny_osm.json"), coords);
    EXPECT_TRUE(hasEdgeTo(g, 1, 2));  // way 100: node2 -> node3 (undirected)
    EXPECT_TRUE(hasEdgeTo(g, 2, 3));  // way 101: node3 -> node4 (oneway forward)
}

TEST(OsmLoader, PlainWayIsUndirected) {
    std::vector<NodeCoord> coords;
    Graph g = loadOsmGraph(fixturePath("tiny_osm.json"), coords);
    EXPECT_TRUE(hasEdgeTo(g, 0, 1));
    EXPECT_TRUE(hasEdgeTo(g, 1, 0)); // reverse direction also present
}

TEST(OsmLoader, OnewayYesIsForwardOnly) {
    std::vector<NodeCoord> coords;
    Graph g = loadOsmGraph(fixturePath("tiny_osm.json"), coords);
    EXPECT_TRUE(hasEdgeTo(g, 3, 4));   // graph node 3 (osm 4) -> graph node 4 (osm 5)
    EXPECT_FALSE(hasEdgeTo(g, 4, 3));  // no reverse edge
}

TEST(OsmLoader, OnewayMinusOneIsReversed) {
    std::vector<NodeCoord> coords;
    Graph g = loadOsmGraph(fixturePath("tiny_osm.json"), coords);
    // way 102 nodes [5, 6] -> graph ids [4, 5]; oneway=-1 means the
    // traversable direction is the reverse of the way's digitized order.
    EXPECT_TRUE(hasEdgeTo(g, 5, 4));
    EXPECT_FALSE(hasEdgeTo(g, 4, 5));
}

TEST(OsmLoader, EdgeWeightMatchesHaversineOfEndpoints) {
    std::vector<NodeCoord> coords;
    Graph g = loadOsmGraph(fixturePath("tiny_osm.json"), coords);
    double expected = haversineMeters(coords[0].lat, coords[0].lng, coords[1].lat, coords[1].lng);

    auto it = std::find_if(g.neighbours(0).begin(), g.neighbours(0).end(),
                            [](const Edge& e) { return e.to == 1; });
    ASSERT_NE(it, g.neighbours(0).end());
    EXPECT_DOUBLE_EQ(it->weight, expected);
}

TEST(OsmLoader, NodeCoordsMatchSourceJson) {
    std::vector<NodeCoord> coords;
    Graph g = loadOsmGraph(fixturePath("tiny_osm.json"), coords);
    EXPECT_DOUBLE_EQ(coords[0].lat, 30.0000);
    EXPECT_DOUBLE_EQ(coords[0].lng, 76.0000);
    EXPECT_DOUBLE_EQ(coords[5].lat, 30.0050);
}
