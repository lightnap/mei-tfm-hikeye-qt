#ifndef C_TEXTURE_RESOURCE_LOADER_H
#define C_TEXTURE_RESOURCE_LOADER_H

#include "common/Math.hpp"
#include "common/Types.hpp"
#include "loadingModule/resourceLoader/CResourceLoader.hpp"

/**
 * @brief Class that loads the texture to put into the terrain.
 */
class CTextureResourceLoader : public CResourceLoader
{
    Q_OBJECT

  public:
    /**
     * @brief Inherited constructor.
     */
    using CResourceLoader::CResourceLoader;

  private:
    /**
     * @brief Loads the corresponding resource.
     * @return Code depicting the loading result.
     */
    Types::eLoadResult LoadResource() override;

    QImage CreateBackgroundTexture();

    /**
     * @brief Draws ground truth over the provided image.
     * @param aImage: The image over which we want to draw the ground truth.
     */
    void DrawGroundTruth(QImage& aImage);

    /**
     * @brief Turns world coordinates into row and column of a pixel in a texture.
     * @param aWorldPoint: World position (in utm) that we want to turn into a pixel position.
     * @param aWorldDomain: Rectangle of world that maps to the texture.
     * @param aTextureSize: Size of texture.
     * @return Vector containing row and column of pixel in image.
     */
    Math::Vector2D<s32> WorldToTexCoords(const Math::Vector2D<f64>& aWorldPoint, const Math::Box2D& aWorldBounds, const QSize& aTextueSize) const;
};

#endif // C_TEXTURE_RESOURCE_LOADER_H
