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

    tNetwork    oNetwork; //!< Ground truth network.
    Math::Box2D oBounds;  //!< Bounding box for the ground truth.

  private:
    std::unordered_map<s64, s64> mBiToUniIndices; //!< Map to indicate repeated tracks in opposite direcions. When tracks A and B are detected to be inverses, we write both indexes
                                                  //!< if A and B in this map. A maps to A and B maps to A.
};

#endif // S_GROUND_TRUTH_H
