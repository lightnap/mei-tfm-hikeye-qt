#ifndef C_MATCHES_RESOURCE_LOADER_H
#define C_MATCHES_RESOURCE_LOADER_H

#include "common/Types.hpp"
#include "loadingModule/resourceLoader/CResourceLoader.hpp"

/**
 * @brief Class that loads the track mathces.
 */
class CMatchesResourceLoader : public CResourceLoader
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
     * @return Code indicating the load result.
     */
    Types::eLoadResult LoadResource() override;
};

#endif // C_MATCHES_RESOURCE_LOADER_H
