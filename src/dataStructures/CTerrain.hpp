#ifndef C_TERRAIN_H
#define C_TERRAIN_H

#include "Types.hpp"

#include <vector>

/**
 * @brief Class that represents a model of the terrain.
 */
class CTerrain
{
  public:
    /**
     * @brief Constructor.
     * @param aHeights: Reference to the height map we will use to construct the terrain.
     */
    CTerrain(const std::vector<f64>& aHeights);
};

#endif // C_TERRAIN_H
