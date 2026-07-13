#include "route_json.h"
#include <gtest/gtest.h>
using namespace route;

// buildRouteJson only needs synthetic points/Tour/RouteLeg data -- no real
// graph or OSM file required, so this tests the JSON shape in isolation.
TEST(RouteJson, ProducesExpectedSchema) {
    std::vector<DeliveryPoint> points = {
        {"A", 1.0, 2.0},
        {"B", 3.0, 4.0},
        {"C", 5.0, 6.0},
    };
    std::vector<int> snappedNodeIds = {10, 20, 30};

    Tour greedy;
    greedy.order = {0, 1, 2};
    greedy.length = 100.0;

    Tour optimized;
    optimized.order = {0, 2, 1};
    optimized.length = 80.0;

    std::vector<RouteLeg> greedyLegs = {
        {0, 1, 40.0, {{1.0, 2.0}, {3.0, 4.0}}},
        {1, 2, 35.0, {{3.0, 4.0}, {5.0, 6.0}}},
        {2, 0, 25.0, {{5.0, 6.0}, {1.0, 2.0}}},
    };
    std::vector<RouteLeg> optimizedLegs = {
        {0, 2, 30.0, {{1.0, 2.0}, {2.5, 3.5}, {5.0, 6.0}}},
        {2, 1, 25.0, {{5.0, 6.0}, {3.0, 4.0}}},
        {1, 0, 25.0, {{3.0, 4.0}, {1.0, 2.0}}},
    };

    nlohmann::json doc = buildRouteJson(points, snappedNodeIds, greedy, optimized, greedyLegs, optimizedLegs);

    ASSERT_EQ(doc["delivery_points"].size(), 3u);
    EXPECT_EQ(doc["delivery_points"][0]["name"], "A");
    EXPECT_DOUBLE_EQ(doc["delivery_points"][0]["lat"].get<double>(), 1.0);
    EXPECT_DOUBLE_EQ(doc["delivery_points"][0]["lng"].get<double>(), 2.0);
    EXPECT_EQ(doc["delivery_points"][0]["snapped_node"], 10);
    EXPECT_EQ(doc["delivery_points"][2]["id"], 2);

    EXPECT_EQ(doc["greedy_route"]["order"], std::vector<int>({0, 1, 2}));
    EXPECT_DOUBLE_EQ(doc["greedy_route"]["distance_m"].get<double>(), 100.0);

    ASSERT_EQ(doc["greedy_route"]["legs"].size(), 3u);
    auto& greedyFirstLeg = doc["greedy_route"]["legs"][0];
    EXPECT_EQ(greedyFirstLeg["from"], 0);
    EXPECT_EQ(greedyFirstLeg["to"], 1);
    EXPECT_DOUBLE_EQ(greedyFirstLeg["distance_m"].get<double>(), 40.0);
    ASSERT_EQ(greedyFirstLeg["polyline"].size(), 2u);

    EXPECT_EQ(doc["optimized_route"]["order"], std::vector<int>({0, 2, 1}));
    EXPECT_DOUBLE_EQ(doc["optimized_route"]["distance_m"].get<double>(), 80.0);

    ASSERT_EQ(doc["optimized_route"]["legs"].size(), 3u);
    auto& firstLeg = doc["optimized_route"]["legs"][0];
    EXPECT_EQ(firstLeg["from"], 0);
    EXPECT_EQ(firstLeg["to"], 2);
    EXPECT_DOUBLE_EQ(firstLeg["distance_m"].get<double>(), 30.0);
    ASSERT_EQ(firstLeg["polyline"].size(), 3u);
    EXPECT_DOUBLE_EQ(firstLeg["polyline"][0][0].get<double>(), 1.0);
    EXPECT_DOUBLE_EQ(firstLeg["polyline"][0][1].get<double>(), 2.0);

    // Greedy and optimized legs must be independent arrays, not aliased.
    EXPECT_NE(greedyFirstLeg["to"], firstLeg["to"]);

    EXPECT_DOUBLE_EQ(doc["improvement_pct"].get<double>(), 20.0);
}

TEST(RouteJson, ImprovementPctIsZeroWhenTiedWithGreedy) {
    std::vector<DeliveryPoint> points = {{"A", 0.0, 0.0}, {"B", 1.0, 1.0}};
    std::vector<int> snappedNodeIds = {1, 2};
    Tour tour;
    tour.order = {0, 1};
    tour.length = 50.0;
    std::vector<RouteLeg> legs = {{0, 1, 50.0, {}}, {1, 0, 0.0, {}}};

    nlohmann::json doc = buildRouteJson(points, snappedNodeIds, tour, tour, legs, legs);
    EXPECT_DOUBLE_EQ(doc["improvement_pct"].get<double>(), 0.0);
}
