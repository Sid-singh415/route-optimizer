#ifndef ROUTE_DELIVERY_POINTS_H
#define ROUTE_DELIVERY_POINTS_H

#include <string>
#include <vector>

namespace route {

struct DeliveryPoint {
    std::string name;
    double lat;
    double lng;
};

// Parses a "name,lat,lng" CSV (with a header row) into delivery points.
std::vector<DeliveryPoint> loadDeliveryPoints(const std::string& csv_path);

} // namespace route

#endif // ROUTE_DELIVERY_POINTS_H
