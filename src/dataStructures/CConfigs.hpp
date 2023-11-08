#ifndef C_CONFIGS_H
#define C_CONFIGS_H

#include "common/Math.hpp"
#include "common/Types.hpp"

/**
 * @brief Structure describing the data we must pass to a height map in order for it to be built.
 */
struct SHeightMapConfig
{
    /**
     * @brief Constructor.
     * @param aMin: Minimum height on the height map.
     * @param aMax: Maximum height on the height map.
     */
    SHeightMapConfig(s32 aMin, s32 aMax)
      : oMinHeight(aMin)
      , oMaxHeight(aMax)
    {
    }

    s32 oMinHeight {0U}; //!< Minimum height in the height map, in meters.
    s32 oMaxHeight {0U}; //!< Maximum height in the height map, in meters.
};

/**
 *@brief Structure describing the data we must pass to a terrain model in order for it to be build.
 */
struct STerrainConfig
{
    f64         oCellSize {0.0}; //!< Real world distance between two points in the height map.
    Math::Box2D oBounds {};      //!< Dimensions of the terrain.
};

#endif // C_CONFIGS_H
