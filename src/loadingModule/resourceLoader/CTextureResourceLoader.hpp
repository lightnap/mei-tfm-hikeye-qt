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
     * Inherited constructor.
     */
    using CResourceLoader::CResourceLoader;

  private:
    /**
     * @brief Loads the corresponding resource.
     * @return Code depicting the loading result.
     */
    Types::eResourceLoadingError LoadResource() override;
};

#endif // C_TEXTURE_RESOURCE_LOADER_H
