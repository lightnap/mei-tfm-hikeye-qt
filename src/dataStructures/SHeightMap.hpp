#ifndef S_HEIGHT_MAP_H
#define S_HEIGHT_MAP_H

#include "common/Math.hpp"
#include "common/Types.hpp"

#include <vector>

class QImage;
struct SHeightMapConfig;

/**
 * @brief Class describing a height map.
 */
struct SHeightMap
{
  public:
    /**
     * @brief Constructror.
     * @param aTexture: Texture from which we create the terrain.
     * @param aConfig: Extra information needed to build the terrain.
     */
    SHeightMap(QImage& aTexture, const SHeightMapConfig& aConfig);

  public:
    s32                 oMaxHeight;  //!< Maximum height of the terrain.
    s32                 oMinHeight;  //!< Minimum height of the terrain.
    Math::Vector2D<u32> oResolution; //!< Resolution of the image.
    std::vector<f64>    oHeights;    //!< Vector containing all heights.
};

#endif // S_HEIGHT_MAP_H
