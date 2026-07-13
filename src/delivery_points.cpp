#include "delivery_points.h"
#include <fstream>
#include <sstream>

namespace route {

std::vector<DeliveryPoint> loadDeliveryPoints(const std::string& csv_path) {
    std::vector<DeliveryPoint> points;
    std::ifstream in(csv_path);

    std::string line;
    std::getline(in, line); // header row

    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string name, latStr, lngStr;
        std::getline(ss, name, ',');
        std::getline(ss, latStr, ',');
        std::getline(ss, lngStr, ',');
        points.push_back({name, std::stod(latStr), std::stod(lngStr)});
    }

    return points;
}

} // namespace route
