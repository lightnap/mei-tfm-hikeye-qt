#ifndef C_TERRAIN_TEXTURE_RESOURCE_LOADER_H
#define C_TERRAIN_TEXTURE_RESOURCE_LOADER_H

#include "common/Types.hpp"
#include "loadingModule/resourceLoader/CResourceLoader.hpp"

/**
 * @brief Class that loads the texture that the terrain must have.
 */
class CTerrainTextureResourceLoader : public CResourceLoader
{
    Q_OBJECT

  public:
    /**
     * @brief Inherited constructor.
     */
    using CResourceLoader::CResourceLoader;

  private:
    /**
     * @brief Loads the corresponding result.
     * @return COde depicting the loading result.
     */
    Types::eLoadResult LoadResource() override;
};

#endif // C_TERRAIN_TEXTURE_RESOURCE_LOADER_H
