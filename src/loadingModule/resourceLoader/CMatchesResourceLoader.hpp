#ifndef C_MATCHES_RESOURCE_LOADER_H
#define C_MATCHES_RESOURCE_LOADER_H

#include "common/Types.hpp"
#include "loadingModule/resourceLoader/CResourceLoader.hpp"

#include <string>
#include <vector>

/**
 * @brief Class that loads the track mathces.
 */
class CMatchesResourceLoader : public CResourceLoader
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

    /**
     * @brief Parses a line of the matxes file to obtain a measurement matched onto the network.
     * @param aCurrentLine: The line in the matches file.
     * @return A vector of the indexes of the edges crossed during the path.
     */
    std::vector<u32> ParseLine(const std::string& aCurrentLine);
};

#endif // C_MATCHES_RESOURCE_LOADER_H
