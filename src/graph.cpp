#include "graph.h"
#include <stdexcept>

namespace route {

Graph::Graph(int num_nodes)
    : num_nodes_(num_nodes), adj_(num_nodes), names_(num_nodes) {}

void Graph::addEdge(int u, int v, double weight, bool undirected) {
    if (u < 0 || u >= num_nodes_ || v < 0 || v >= num_nodes_)
        throw std::out_of_range("addEdge: node id out of range");
    adj_[u].push_back({v, weight});
    if (undirected)
        adj_[v].push_back({u, weight});
}

const std::vector<Edge>& Graph::neighbours(int u) const {
    if (u < 0 || u >= num_nodes_)
        throw std::out_of_range("neighbours: node id out of range");
    return adj_[u];
}

int Graph::addNamedNode(const std::string& name) {
    for (int i = 0; i < num_nodes_; ++i)
        if (names_[i] == name) return i;
    // Grow by one node.
    adj_.emplace_back();
    names_.push_back(name);
    return num_nodes_++;
}

int Graph::idOf(const std::string& name) const {
    for (int i = 0; i < num_nodes_; ++i)
        if (names_[i] == name) return i;
    return -1;
}

const std::string& Graph::nameOf(int id) const {
    if (id < 0 || id >= num_nodes_)
        throw std::out_of_range("nameOf: node id out of range");
    return names_[id];
}

} // namespace route
