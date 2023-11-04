#ifndef C_TEXTURE_RESOURCE_LOADER_H
#define C_TEXTURE_RESOURCE_LOADER_H

#include "CResourceLoader.hpp"
#include "Types.hpp"

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

    /**
     * @brief Turns world coordinates into row and column of a pixel in a texture.
     * @param aWorldPoint: World position (in utm) that we want to turn into a pixel position.
     * @param aWorldDomain: Rectangle of world that maps to the texture.
     * @param aTextureSize: Size of texture.
     */
    void WorldToTexCoords(const Math::Vector2D& aWorldPoint, const Math::Box2D& aWorldBounds, const QSize& aTextueSize, s32& texi, s32& texj) const;
};

#endif // C_TEXTURE_RESOURCE_LOADER_H
