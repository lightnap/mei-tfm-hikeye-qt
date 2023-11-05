#ifndef C_GROUND_TRUTH_RESOURCE_LOADER_H
#define C_GROUND_TRUTH_RESOURCE_LOADER_H

#include "CResourceLoader.hpp"
#include "SGroundTruth.hpp"
#include "Types.hpp"

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

    /**
     * @brief Fills the given ground truth data with data from the given xml.
     * @param aXml: The given Xml where the ground truth data is stored.
     * @param aNetwork: Reference to the network we want to fill.
     * @return LoadResult indicating if the load was sucessfull.
     */
    Types::eLoadResult FillGroundTruth(QXmlStreamReader& aXml, SGroundTruth::tNetwork& aNetwork);

    SGroundTruth::tTrack ParseWay(QXmlStreamReader& aXml);
};

#endif // C_GROUND_TRUTH_RESOURCE_LOADER_H
