#ifndef SMATCHES_H
#define SMATCHES_H

#include "common/Types.hpp"

#include <vector>

/**
 * @brief Class that represents a collection of matches; that is, measurements matched to edges on the ground truth.
 */
struct SMatches
{
  public:
    using tRouteMatch = std::vector<u32>;      //!< Type that represents a set of measurements matched onto the edge ids of the ground network.
    using tMatches = std::vector<tRouteMatch>; //!< Type that represents a list of matches.

    /**
     * @brief Constructor.
     * @param aMatches: List of matches we want to save.
     */
    SMatches(const tMatches&& aMatches);

    tMatches oMatches; //!< Matches.
};

#endif // SMATCHES_H
