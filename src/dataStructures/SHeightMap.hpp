#ifndef S_HEIGHT_MAP_H
#define S_HEIGHT_MAP_H

#include "Types.hpp"

#include <vector>

class QImage;

/**
 * @brief Class describing a height map.
 */
struct SHeightMap
{
  public:
    struct SResolution
    {
        u32 oX {0U};
        u32 oY {0U};
    };

  public:
    /**
     * @brief Constructror.
     * @param aTexture: Texture from which we create the terrain.
     */
    SHeightMap(QImage& aTexture);

  public:
    std::vector<f64> oHeights;    //!< Vector containing all heights.
    SResolution      oResolution; //!< Resolution of the image.
};

#endif // S_HEIGHT_MAP_H
