#include "CQueriesResourceLoader.hpp"

#include "common/Types.hpp"
#include "dataStructures/SQueries.hpp"
#include "loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp"

#include <iostream>
#include <memory>
#include <utility>

namespace
{
    [[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CQueriesResourceLoader>::Register(Types::eResource::Queries)};
}

Types::eLoadResult CQueriesResourceLoader::LoadResource()
{
    std::cout << "[QueriesResource] Loading queries" << std::endl;
    // TODO: HK-24 Fill this function.
    const auto& GroundTruth {mDataManager.GetGroundTruth()};

    SQueries::tCrossingCount CrossingCount;

    const auto& Matches {mDataManager.GetMatches()};

    for (const auto& Route : Matches.oMatches)
    {
        for (const auto Edge : Route)
        {
            const auto UnidirectionalEdgeIndex {GroundTruth.BidirectionalToUnidirectional(Edge)};

            const auto& EdgeCrossingCountIt {CrossingCount.find(UnidirectionalEdgeIndex)};
            if (EdgeCrossingCountIt != CrossingCount.end())
            {
                EdgeCrossingCountIt->second++;
            }
            else
            {
                CrossingCount[UnidirectionalEdgeIndex] = 1;
            }
        }
    }

    // TODO: Remove this.
    // std::cout << "Printing crossing count:" << std::endl;
    // for (const auto Count : CrossingCount)
    //{
    // std::cout << "[" << Count.first << "," << Count.second << "], ";
    //}
    // std::cout << std::endl;

    // TODO: Remove this.
    // Print crossings count historigram.
    std::map<s32, s32> CrossingsCountHistogram;
    for (const auto& [_, Count] : CrossingCount)
    {
        const auto HistogramIt {CrossingsCountHistogram.find(Count)};
        if (HistogramIt == CrossingsCountHistogram.end())
        {
            CrossingsCountHistogram[Count] = 1;
        }
        else
        {
            HistogramIt->second++;
        }
    }

    std::cout << "Printing CrossingsCount Hisogram:" << std::endl;

    for (const auto& [Crossings, TimesItHappened] : CrossingsCountHistogram)
    {
        std::cout << "A track crossed: " << Crossings << " was found: " << TimesItHappened << " times" << std::endl;
        // std::cout << Crossings << std::endl;
    }

    std::unique_ptr<SQueries> Queries = std::make_unique<SQueries>(std::move(CrossingCount));
    mDataManager.SetQueries(std::move(Queries));

    return Types::eLoadResult::Successful;
}
