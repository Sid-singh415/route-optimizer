#ifndef ROUTE_ROUTE_JSON_H
#define ROUTE_ROUTE_JSON_H

#include "delivery_points.h"
#include "osm_loader.h"
#include "nearest_neighbour.h"
#include <nlohmann/json.hpp>
#include <vector>

namespace route {

struct RouteLeg {
    int from;
    int to;
    double distance_m;
    std::vector<NodeCoord> polyline;
};

// Builds the route.json document: each delivery point (with its snapped
// graph node id), and both the greedy and optimized routes' order + distance
// + per-leg road-following polylines.
nlohmann::json buildRouteJson(
    const std::vector<DeliveryPoint>& points,
    const std::vector<int>& snappedNodeIds,
    const Tour& greedy,
    const Tour& optimized,
    const std::vector<RouteLeg>& greedyLegs,
    const std::vector<RouteLeg>& optimizedLegs);

} // namespace route

#endif // ROUTE_ROUTE_JSON_H
