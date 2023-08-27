#ifndef C_TEXTURE_RESOURCE_LOADER_H
#define C_TEXTURE_RESOURCE_LOADER_H

#include "CResourceLoader.hpp"

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
     */
    void LoadResource() override;
};

#endif // C_TEXTURE_RESOURCE_LOADER_H
