#ifndef S_GROUND_TRUTH_H
#define S_GROUND_TRUTH_H

#include "Math.hpp"
#include "Types.hpp"

#include <vector>

/**
 * @brief Class that represents a ground truth network.
 */
struct SGroundTruth
{
  public:
    using tPoint = Math::Vector2D<f64>;   //!< Type to represent a point in the track (UTM coordinates).
    using tTrack = std::vector<tPoint>;   //!< Type to represent a track in the network.
    using tNetwork = std::vector<tTrack>; //!< Type to represent the ground truth network.

  public:
    tNetwork    oNetwork; //!< Ground truth network.
    Math::Box2D oBounds;  //!< Bounding box for the ground truth.
};

#endif // S_GROUND_TRUTH_H
