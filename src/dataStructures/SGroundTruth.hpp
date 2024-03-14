#ifndef S_GROUND_TRUTH_H
#define S_GROUND_TRUTH_H

#include "common/Math.hpp"
#include "common/Types.hpp"

#include <unordered_map>
#include <vector>

/**
 * @brief Class that represents a ground truth network.
 */
struct SGroundTruth
{
  public:
    /**
     * @brief Struct that represents a point in UTM coordinates.
     */
    struct SPoint
    {
        f64 oNorthing {0.0f}; //!< Northing coordinate of point.
        f64 oEasting {0.0f};  //!< Easting coordiate of point.
        s64 oOsmId {-1};      //!< Osm Identifier of point.
    };

    /**
     * @brief Struct that represents a track: a collection of points.
     */
    struct STrack
    {
        std::vector<SPoint> oPoints; //!< Vector of points.
    };

    using tNetwork = std::vector<STrack>; //!< Type to represent the ground truth network.

    /**
     * @brief Constructor.
     * @param aNetwork: Ground truth network we want to save.
     */
    SGroundTruth(const tNetwork&& aNetwork);

    /**
     * @brief Gets the index of the track in the unidirectional network corresponing to the track index in the bidirectional network.
     * @param aTrackIndex: The track's index.
     * @return The track.
     */
    s64 BidirectionalToUnidirectional(s64 aTrackIndex) const;

    /**
     * @brieg For a given (bidirectional) track index, get the direction of the track.
     * @param aTrackIndex The bidirectional track index.
     * @return The direcion of said track. Positive if Osm of endpoint is bigger than osm id of start point.
     */
    Types::eDirection GetDirection(s64 aTrackIndex) const;

    tNetwork    oNetwork; //!< Ground truth network.
    Math::Box2D oBounds;  //!< Bounding box for the ground truth.

  private:
    std::unordered_map<s64, s64>               mBiToUniIndices; //!< Maps track indexes to position in oNetwork vector.
    std::unordered_map<s64, Types::eDirection> mDirections;     //!< Keeps track of firection of each track.
};

#endif // S_GROUND_TRUTH_H
