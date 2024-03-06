#ifndef C_QUERIES_RESOURCE_LOADER_H
#define C_QUERIES_RESOURCE_LOADER_H

#include "common/Types.hpp"
#include "dataStructures/SMatches.hpp"
#include "loadingModule/resourceLoader/CResourceLoader.hpp"

/**
 * @brief Class that enriches the ground truth with data through queries.
 */
class CQueriesResourceLoader : public CResourceLoader
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
     * @brief Decides whether this match is part of the query.
     * @param aMatch: The match we are considering
     * @return True if we must discard this match, false otherwise.
     */
    bool MustFilter(const SMatches::SMatch& aMatch) const;
};

#endif // C_QUERIES_RESOURCE_LOADER_H
