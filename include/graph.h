#ifndef ROUTE_GRAPH_H
#define ROUTE_GRAPH_H

#include <string>
#include <vector>
#include <limits>

namespace route {

// Sentinel for "no path / infinite distance".
constexpr double INF = std::numeric_limits<double>::infinity();

// A single weighted directed edge (u -> v is stored on u's adjacency list).
struct Edge {
    int to;
    double weight;
};

// Weighted graph using an adjacency list.
// Nodes are integer IDs [0, n). Optional human-readable names map to IDs
// (used later by CSV I/O and the optional NL layer).
class Graph {
public:
    explicit Graph(int num_nodes);

    // Add an edge. If undirected, adds both directions.
    void addEdge(int u, int v, double weight, bool undirected = true);

    int numNodes() const { return num_nodes_; }

    // Neighbours of u, for algorithms to iterate over.
    const std::vector<Edge>& neighbours(int u) const;

    // --- Name <-> id helpers (thin; fine to ignore until CSV/NL work) ---
    int  addNamedNode(const std::string& name); // returns id, reuses if exists
    int  idOf(const std::string& name) const;   // -1 if not found
    const std::string& nameOf(int id) const;

private:
    int num_nodes_;
    std::vector<std::vector<Edge>> adj_;
    std::vector<std::string> names_;
};

} // namespace route

#endif // ROUTE_GRAPH_H
