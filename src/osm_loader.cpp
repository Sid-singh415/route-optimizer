#include "osm_loader.h"
#include "geo_utils.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <unordered_map>

namespace route {

Graph loadOsmGraph(const std::string& json_path, std::vector<NodeCoord>& coords) {
    std::ifstream in(json_path);
    nlohmann::json doc;
    in >> doc;

    // Pass A: every OSM node's raw lat/lng, keyed by its OSM id.
    std::unordered_map<int64_t, NodeCoord> rawCoords;
    for (const auto& el : doc["elements"]) {
        if (el["type"] == "node") {
            rawCoords[el["id"].get<int64_t>()] = {el["lat"].get<double>(), el["lon"].get<double>()};
        }
    }

    // Pass B: assign a dense graph node id to every OSM node actually
    // referenced by a way, so a node shared by two ways collapses to the
    // same graph id (that's what makes intersections connect).
    std::unordered_map<int64_t, int> idMap;
    coords.clear();
    for (const auto& el : doc["elements"]) {
        if (el["type"] != "way") continue;
        for (const auto& nodeId : el["nodes"]) {
            int64_t id = nodeId.get<int64_t>();
            if (idMap.find(id) == idMap.end()) {
                idMap[id] = static_cast<int>(coords.size());
                coords.push_back(rawCoords.at(id));
            }
        }
    }

    Graph g(static_cast<int>(coords.size()));

    // Pass C: one edge per consecutive node pair along each way.
    for (const auto& el : doc["elements"]) {
        if (el["type"] != "way") continue;
        std::string oneway = el.value("tags", nlohmann::json::object()).value("oneway", "");
        const auto& nodes = el["nodes"];
        for (size_t i = 0; i + 1 < nodes.size(); ++i) {
            int u = idMap.at(nodes[i].get<int64_t>());
            int v = idMap.at(nodes[i + 1].get<int64_t>());
            double weight = haversineMeters(coords[u].lat, coords[u].lng, coords[v].lat, coords[v].lng);

            if (oneway == "yes" || oneway == "1" || oneway == "true") {
                g.addEdge(u, v, weight, /*undirected=*/false);
            } else if (oneway == "-1") {
                g.addEdge(v, u, weight, /*undirected=*/false);
            } else {
                g.addEdge(u, v, weight, /*undirected=*/true);
            }
        }
    }

    return g;
}

} // namespace route
