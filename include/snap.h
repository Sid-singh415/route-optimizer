#ifndef ROUTE_SNAP_H
#define ROUTE_SNAP_H

#include "osm_loader.h"
#include <vector>

namespace route {

// Returns the index into `coords` of the nearest node to (lat, lng),
// by plain linear scan (haversine distance to every candidate).
int nearestNode(const std::vector<NodeCoord>& coords, double lat, double lng);

} // namespace route

#endif // ROUTE_SNAP_H
