#include "route_json.h"

namespace route {

static nlohmann::json legsToJson(const std::vector<RouteLeg>& legs) {
    nlohmann::json legsJson = nlohmann::json::array();
    for (const auto& leg : legs) {
        nlohmann::json polyline = nlohmann::json::array();
        for (const auto& c : leg.polyline) polyline.push_back({c.lat, c.lng});
        legsJson.push_back({
            {"from", leg.from},
            {"to", leg.to},
            {"distance_m", leg.distance_m},
            {"polyline", polyline}
        });
    }
    return legsJson;
}

nlohmann::json buildRouteJson(
    const std::vector<DeliveryPoint>& points,
    const std::vector<int>& snappedNodeIds,
    const Tour& greedy,
    const Tour& optimized,
    const std::vector<RouteLeg>& greedyLegs,
    const std::vector<RouteLeg>& optimizedLegs) {

    nlohmann::json deliveryPoints = nlohmann::json::array();
    for (size_t i = 0; i < points.size(); ++i) {
        deliveryPoints.push_back({
            {"id", static_cast<int>(i)},
            {"name", points[i].name},
            {"lat", points[i].lat},
            {"lng", points[i].lng},
            {"snapped_node", snappedNodeIds[i]}
        });
    }

    nlohmann::json doc;
    doc["delivery_points"] = deliveryPoints;
    doc["greedy_route"] = {
        {"order", greedy.order},
        {"distance_m", greedy.length},
        {"legs", legsToJson(greedyLegs)}
    };
    doc["optimized_route"] = {
        {"order", optimized.order},
        {"distance_m", optimized.length},
        {"legs", legsToJson(optimizedLegs)}
    };
    doc["improvement_pct"] = 100.0 * (greedy.length - optimized.length) / greedy.length;

    return doc;
}

} // namespace route
