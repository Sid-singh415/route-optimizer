#ifndef ROUTE_TWO_OPT_H
#define ROUTE_TWO_OPT_H

#include "nearest_neighbour.h"

namespace route {

// Improve an existing tour with 2-opt local search: repeatedly reverse a
// segment if doing so shortens the tour, until no improving move remains.
// Returns the improved tour (length <= input length).
Tour twoOpt(const AllPairs& ap, Tour tour);

} // namespace route

#endif // ROUTE_TWO_OPT_H
