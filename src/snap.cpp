#include "snap.h"
#include "geo_utils.h"

namespace route {

int nearestNode(const std::vector<NodeCoord>& coords, double lat, double lng) {
    int best = -1;
    double bestDist = INF;
    for (size_t i = 0; i < coords.size(); ++i) {
        double d = haversineMeters(lat, lng, coords[i].lat, coords[i].lng);
        if (d < bestDist) {
            bestDist = d;
            best = static_cast<int>(i);
        }
    }
    return best;
}

} // namespace route
