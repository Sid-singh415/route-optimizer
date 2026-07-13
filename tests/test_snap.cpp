#include "snap.h"
#include <gtest/gtest.h>
using namespace route;

TEST(Snap, PicksClosestOfSeveralCandidates) {
    std::vector<NodeCoord> coords = {
        {30.0000, 76.0000},
        {30.0100, 76.0000}, // ~1.1km away
        {30.0005, 76.0000}, // ~55m away -- closest
        {31.0000, 77.0000}, // far away
    };
    int result = nearestNode(coords, 30.0000, 76.0000);
    EXPECT_EQ(result, 0); // exact match wins
}

TEST(Snap, FindsNearestWhenNoExactMatch) {
    std::vector<NodeCoord> coords = {
        {30.0000, 76.0000},
        {30.0100, 76.0000},
        {31.0000, 77.0000},
    };
    // Query point is closest to index 0.
    int result = nearestNode(coords, 30.0004, 76.0000);
    EXPECT_EQ(result, 0);
}

TEST(Snap, SingleCandidateAlwaysWins) {
    std::vector<NodeCoord> coords = {{12.34, 56.78}};
    EXPECT_EQ(nearestNode(coords, 0.0, 0.0), 0);
}
