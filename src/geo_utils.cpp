#include "geo_utils.h"
#include <cmath>

namespace route {

double haversineMeters(double lat1, double lon1, double lat2, double lon2) {
    constexpr double kEarthRadiusMeters = 6371000.0;
    constexpr double kDegToRad = M_PI / 180.0;

    double dLat = (lat2 - lat1) * kDegToRad;
    double dLon = (lon2 - lon1) * kDegToRad;
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1 * kDegToRad) * std::cos(lat2 * kDegToRad) *
                   std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return kEarthRadiusMeters * c;
}

} // namespace route
