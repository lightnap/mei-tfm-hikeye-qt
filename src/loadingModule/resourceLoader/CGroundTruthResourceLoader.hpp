#ifndef C_GROUND_TRUTH_RESOURCE_LOADER_H
#define C_GROUND_TRUTH_RESOURCE_LOADER_H

#include "CResourceLoader.hpp"

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
     */
    virtual void LoadResource();
};

#endif // C_GROUND_TRUTH_RESOURCE_LOADER_H
