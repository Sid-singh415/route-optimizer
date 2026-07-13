#include "osm_loader.h"
#include "delivery_points.h"
#include "snap.h"
#include "dijkstra.h"
#include "nearest_neighbour.h"
#include "two_opt.h"
#include "route_json.h"
#include <iostream>
#include <fstream>
#include <numeric>

using namespace route;

int main() {
    std::vector<NodeCoord> coords;
    Graph g = loadOsmGraph("data/chandigarh_roads.json", coords);
    std::cout << "Loaded road graph: " << g.numNodes() << " nodes\n";

    auto points = loadDeliveryPoints("data/delivery_points.csv");
    int n = static_cast<int>(points.size());

    std::vector<int> deliveryNodeId(n);
    for (int i = 0; i < n; ++i) {
        deliveryNodeId[i] = nearestNode(coords, points[i].lat, points[i].lng);
        std::cout << "Snapped \"" << points[i].name << "\" to node " << deliveryNodeId[i] << "\n";
    }

    // Small N x N matrix among just the delivery points (local indices 0..n-1),
    // NOT the full road graph's node ids -- nearestNeighbour/twoOpt index
    // ap.dist using the values inside `stops`/`tour.order` directly.
    AllPairs ap;
    ap.n = n;
    ap.dist.assign(n, std::vector<double>(n, 0.0));

    // Node-id sequence of the real road path for every ordered pair, captured
    // from the same Dijkstra runs used to build the matrix (no extra runs).
    std::vector<std::vector<std::vector<int>>> pathNodes(n, std::vector<std::vector<int>>(n));

    for (int i = 0; i < n; ++i) {
        ShortestPaths sp = dijkstra(g, deliveryNodeId[i]);
        for (int j = 0; j < n; ++j) {
            ap.dist[i][j] = sp.dist[deliveryNodeId[j]];
            if (i != j) pathNodes[i][j] = reconstructPath(sp, deliveryNodeId[i], deliveryNodeId[j]);
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && ap.dist[i][j] == INF) {
                std::cerr << "Unreachable: " << points[i].name << " <-> " << points[j].name
                          << " (disconnected road network component)\n";
                return 1;
            }
        }
    }

    std::vector<int> stops(n);
    std::iota(stops.begin(), stops.end(), 0);

    Tour greedy = nearestNeighbour(ap, stops, 0);
    Tour optimized = twoOpt(ap, greedy);

    std::cout << "\nGreedy route:    ";
    for (int idx : greedy.order) std::cout << points[idx].name << " -> ";
    std::cout << points[greedy.order.front()].name << "\n";
    std::cout << "Greedy distance: " << greedy.length << " m\n";

    std::cout << "\nOptimized route: ";
    for (int idx : optimized.order) std::cout << points[idx].name << " -> ";
    std::cout << points[optimized.order.front()].name << "\n";
    std::cout << "Optimized distance: " << optimized.length << " m\n";

    double improvement = 100.0 * (greedy.length - optimized.length) / greedy.length;
    std::cout << "\nImprovement: " << improvement << "%\n";

    // One leg per consecutive pair in a given order, closing the loop back to
    // the start -- polylines come straight from the pathNodes captured during
    // the distance-matrix Dijkstra runs above (covers every pair, so this
    // works for both the greedy and optimized orders with no extra work).
    auto buildLegs = [&](const std::vector<int>& order) {
        std::vector<RouteLeg> legs;
        for (size_t i = 0; i < order.size(); ++i) {
            int a = order[i];
            int b = order[(i + 1) % order.size()];
            RouteLeg leg;
            leg.from = a;
            leg.to = b;
            leg.distance_m = ap.dist[a][b];
            for (int nodeId : pathNodes[a][b]) leg.polyline.push_back(coords[nodeId]);
            legs.push_back(leg);
        }
        return legs;
    };

    std::vector<RouteLeg> greedyLegs = buildLegs(greedy.order);
    std::vector<RouteLeg> optimizedLegs = buildLegs(optimized.order);

    nlohmann::json doc = buildRouteJson(points, deliveryNodeId, greedy, optimized, greedyLegs, optimizedLegs);
    std::ofstream out("route.json");
    out << doc.dump(2);
    std::cout << "\nWrote route.json\n";

    return 0;
}
