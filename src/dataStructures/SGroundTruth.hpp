#ifndef S_GROUND_TRUTH_H
#define S_GROUND_TRUTH_H

#include "common/Math.hpp"
#include "common/Types.hpp"

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

    tNetwork    oNetwork; //!< Ground truth network.
    Math::Box2D oBounds;  //!< Bounding box for the ground truth.
};

#endif // S_GROUND_TRUTH_H
