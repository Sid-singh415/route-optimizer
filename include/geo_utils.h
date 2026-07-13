#ifndef ROUTE_GEO_UTILS_H
#define ROUTE_GEO_UTILS_H

namespace route {

// Great-circle distance between two lat/lng points (decimal degrees), in meters.
double haversineMeters(double lat1, double lon1, double lat2, double lon2);

} // namespace route

#endif // ROUTE_GEO_UTILS_H
