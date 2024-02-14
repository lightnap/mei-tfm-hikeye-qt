#ifndef C_SAVEIMAGE_RESOURCE_LOADER_H
#define C_SAVEIMAGE_RESOURCE_LOADER_H

#include "common/Types.hpp"
#include "loadingModule/resourceLoader/CResourceLoader.hpp"

#include <string>
#include <vector>

/**
 * @brief Class that saves the tracks texture image into file..
 */
class CSaveImageResourceLoader : public CResourceLoader
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
     * @return Code indicating the load result.
     */
    Types::eLoadResult LoadResource() override;
};

#endif // C_SAVEIMAGE_RESOURCE_LOADER_H
