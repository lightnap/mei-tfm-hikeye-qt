#ifndef C_HEIGHT_MAP_RESOURCE_LOADER_H
#define C_HEIGHT_MAP_RESOURCE_LOADER_H

#include "CResourceLoader.hpp"
#include "Types.hpp"

/**
 * @brief Class that laods the Height map image.
 */
class CHeightMapResourceLoader : public CResourceLoader
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
     * @return Error code depicting the loading result.
     */
    Types::eResourceLoadingError LoadResource() override;
};

#endif // C_HEIGHT_MAP_RESOURCE_LOADER_H
