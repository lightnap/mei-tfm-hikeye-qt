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
    /**
     * @brief Struct to represent all the info we have from a person crossing a track.
     */
    struct SCrossingInfo
    {
        /**
         * @brief Enum describing the direction of the crossing.
         */
        enum class eDirection
        {
            None = 0, //!< Unknown direction.
            Positive, //!< Destination node had higher osmId than source node.
            Negative, //!< Destination node had lower osmId than source node.
        };

        int32_t    MatchIndex {-1};              //!< Id of GPS Track from which this record came.
        eDirection Direction {eDirection::None}; //!< Direction of the crossing.
        uint64_t   Date {0U};                    //!< Date when crossing happened.
        double     Speed {0.0};                  //!< Speed at which the crossing happened.
    };

    using tCrossingsInfo = std::multimap<s64, SCrossingInfo>; //!< Type that represents info on how each track was crossed.

    /**
     * @brief Constructor.
     * @param aCrossingsInfo: Info on each crossing.
     */
    SQueries(const tCrossingsInfo&& aCrossingsInfo);

    tCrossingsInfo oCrossingsInfo; //!< For each track, info on all crossings we have of it,
};

#endif // SQUERIES_H
