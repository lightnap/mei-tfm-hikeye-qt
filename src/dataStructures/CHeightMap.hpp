#ifndef C_HEIGHT_MAP_H
#define C_HEIGHT_MAP_H

#include "Types.hpp"

#include <vector>

class QImage;

/**
 * @brief Class describing a height map.
 */
class CHeightMap
{
  public:
    /**
     * @brief Constructror.
     * @param aTexture: Texture from which we create the terrain.
     */
    CHeightMap(QImage& aTexture);

  private:
    std::vector<f64> mHeights; //!< Vector containing all heights.
};

#endif // C_HEIGHT_MAP_H
