#include "two_opt.h"
#include <algorithm>   // for std::reverse

namespace route {

// Improve a tour with 2-opt: repeatedly reverse a segment whenever doing so
// shortens the tour, until a full pass finds no improvement.
Tour twoOpt(const AllPairs& ap, Tour tour) {
    std::vector<int>& r = tour.order;   // reference: edits to r change tour.order
    int n = r.size();
    if (n < 4) return tour;             // nothing to swap on tiny tours

    bool improved = true;
    while (improved) {
        improved = false;

        // try every pair of positions (i, k) in the tour
        for (int i = 1; i < n - 1; ++i) {
            for (int k = i + 1; k < n; ++k) {

                // The 4 nodes involved in the potential swap:
                //   ... a -> b ............ c -> d ...
                //   b is r[i], c is r[k]. a is the node BEFORE b, d is the node AFTER c.
                int a = r[i - 1];
                int b = r[i];
                int c = r[k];
                int d = r[(k + 1) % n];   // % n so the last position wraps to the first

                // current cost of the two edges we'd remove: a->b and c->d
                double before = ap.dist[a][b] + ap.dist[c][d];   // blanks 1 & 2

                // cost if we reconnect the other way: a->c and b->d
                double after  = ap.dist[a][c] + ap.dist[b][d];   // blanks 3 & 4

                // if the new way is strictly cheaper, do the swap
                if (after < before) {
                    // reverse the segment between i and k (inclusive)
                    std::reverse(r.begin() + i, r.begin() + k + 1);
                    improved =true;                                   // blank 5
                }
            }
        }
    }

    // recompute the final length from scratch (safest)
    tour.length = tourLength(ap, tour.order);
    return tour;
}

} // namespace route