#ifndef C_GROUND_TRUTH_RESOURCE_LOADER_H
#define C_GROUND_TRUTH_RESOURCE_LOADER_H

#include "common/Types.hpp"
#include "dataStructures/SGroundTruth.hpp"
#include "loadingModule/resourceLoader/CResourceLoader.hpp"

class QXmlStreamReader;

/**
 * @brief Class that loads the ground truth.
 */
class CGroundTruthResourceLoader : public CResourceLoader
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
    Types::eLoadResult LoadResource() override;
};

#endif // C_GROUND_TRUTH_RESOURCE_LOADER_H
