#ifndef S_HEIGHT_MAP_H
#define S_HEIGHT_MAP_H

#include "Types.hpp"

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
     * @brief Structrure to save the resolution of the image.
     */
    struct SResolution // TODO: Turn this into a Vector2D<u32>
    {
        u32 oX {0U}; //!< Number of pixels on the X dimension.
        u32 oY {0U}; //!< Number of pixels on the Y dimension.
    };

  public:
    /**
     * @brief Constructror.
     * @param aTexture: Texture from which we create the terrain.
     * @param aConfig: Extra information needed to build the terrain.
     */
    SHeightMap(QImage& aTexture, const SHeightMapConfig& aConfig);

  public:
    std::vector<f64> oHeights;    //!< Vector containing all heights.
    SResolution      oResolution; //!< Resolution of the image.
    s32              oMaxHeight;  //!< Maximum height of the terrain.
    s32              oMinHeight;  //!< Minimum height of the terrain.
};

#endif // S_HEIGHT_MAP_H
