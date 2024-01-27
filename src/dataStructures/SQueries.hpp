#ifndef SQUERIES_H
#define SQUERIES_H

#include "common/Types.hpp"

#include <map>

/**
 * @brief Class that represents queries to the ground truth; that is; information over each edge obtained from processing the matches.
 */
struct SQueries
{
  public:
    using tCrossingCount = std::map<s64, u32>; //!< Type that represents a query in which we count how many people crossed an edge (identified by its index).

    /**
     * @brief Constructor.
     * @param aCrossingCount: Query in which we count how many time and edge was traversed.
     */
    SQueries(const tCrossingCount&& aCrossingCount);

    tCrossingCount oCrossingCount; //!< Query in which we count how many times and edge was traversed.
};

#endif // SQUERIES_H
