#ifndef ROUTE_OSM_LOADER_H
#define ROUTE_OSM_LOADER_H

#include "graph.h"
#include <string>
#include <vector>

namespace route {

// Lat/lng of a single graph node, indexed the same as Graph's node ids.
struct NodeCoord {
    double lat;
    double lng;
};

// Parses a cached Overpass "[out:json]" response (ways + their nodes) at
// `json_path` into a Graph. Every OSM node referenced by a matching way
// becomes its own graph node (including shape points, not just true
// intersections) so that reconstructPath() can later return a full
// road-following polyline with no extra bookkeeping. `coords` is resized to
// match the returned graph's numNodes(), coords[i] giving node i's lat/lng.
Graph loadOsmGraph(const std::string& json_path, std::vector<NodeCoord>& coords);

} // namespace route

#endif // ROUTE_OSM_LOADER_H
