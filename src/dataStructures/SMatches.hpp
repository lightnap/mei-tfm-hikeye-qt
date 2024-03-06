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
    /**
     * @brief Struct that represents a single matc.
     */
    struct SMatch
    {
        std::vector<u32> EdgeIndices {}; //!< Indices of the ground truth.
        s64              Date {0L};      //!< Date at which the track was recorded,
    };

    using tMatches = std::vector<SMatch>; //!< Type that represents a list of matches.
    /**
     * @brief Constructor.
     * @param aMatches: List of matches we want to save.
     */
    SMatches(const tMatches&& aMatches);

    tMatches oMatches; //!< Matches.
};

#endif // SMATCHES_H
