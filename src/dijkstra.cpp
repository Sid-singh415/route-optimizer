#include "dijkstra.h"
#include <queue>
#include <vector>
#include <set>
using namespace std;

namespace route {

// ============================================================================
//  YOUR TASK: implement Dijkstra with a binary min-heap.
//
//  SPEC / STATE:
//    - dist[v]   : best known distance from source to v   (init: INF, dist[source]=0)
//    - parent[v] : predecessor of v on the best path      (init: -1)
//    - a min-heap (std::priority_queue) of (distance, node) pairs
//
//  THE CORE LOOP:
//    1. Push (0, source) onto the heap.
//    2. Pop the smallest-distance node. If its popped distance is stale
//       (greater than dist[node]), skip it  <-- this is the "lazy deletion"
//       trick that lets us avoid a decrease-key operation. Think about WHY
//       that check is correct.
//    3. For each neighbour, try to relax the edge:
//         if dist[node] + w < dist[neighbour]  -> update dist + parent,
//         push the new (dist, neighbour) pair.
//
//  HEAP HINT:
//    std::priority_queue is a MAX-heap by default. To get a min-heap of pairs:
//      using PQItem = std::pair<double,int>;   // (distance, node)
//      std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;
//    Pairs compare lexicographically, so (distance, node) orders by distance
//    first — exactly what you want.
//
//  COMPLEXITY TO BE ABLE TO EXPLAIN IN INTERVIEW:
//    O((V + E) log V) with a binary heap. Be ready to say where the log V
//    comes from and why lazy deletion doesn't change the bound.
// ============================================================================

ShortestPaths dijkstra(const Graph& g, int source) {
    int n = g.numNodes();
    ShortestPaths sp;
    sp.dist.assign(n, INF);
    sp.parent.assign(n, -1);

    
    // 1. source ki distance 0 karo
    sp.dist[source] = 0;

    // 2. set of (distance, node), sorted automatically
    set<pair<double,int>> pq;
    pq.insert({sp.dist[source],source});   // source daalo

    // 3. jab tak set khaali nahi
    while (!pq.empty()) {
        // closest unprocessed node nikaalo (set ka first element)
        auto [d, u] = *pq.begin();
        pq.erase(pq.begin());

        // 4. u ke har neighbour ko dekho
        for (const Edge& e : g.neighbours(u)) {
            int v = e.to;
            double w = e.weight;

            // relax: kya u ke through v tak jaana sasta hai?
            if (sp.dist[u] + w < sp.dist[v]) {
                // set se purani entry hatao (agar thi)
                pq.erase({sp.dist[v], v});

                sp.dist[v] = sp.dist[u] + w;        // nayi distance
                sp.parent[v] = u;      // v kahaan se aaya
                pq.insert({sp.dist[v],v});  // updated entry daalo
            }
        }
    }
    
    return sp;
}

// ----------------------------------------------------------------------------
//  YOUR TASK: walk parent[] backwards from target to source, then reverse.
//    - If sp.dist[target] == INF, return {} (unreachable).
//    - Otherwise collect target, parent[target], ... until you hit source,
//      then reverse so the path reads source -> ... -> target.
// ----------------------------------------------------------------------------
std::vector<int> reconstructPath(const ShortestPaths& sp, int source, int target) {
    std::vector<int> path;

    // 1. agar target reachable hi nahi hai, khaali return karo
    if (sp.dist[target] == INF)
        return path;

    int curr=target;
    while(curr!=-1){
        path.push_back(curr);
        curr=sp.parent[curr];
    }
   
    reverse(path.begin(),path.end());
    return path;
}

} // namespace route
