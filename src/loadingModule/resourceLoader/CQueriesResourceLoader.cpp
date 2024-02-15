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
    const auto& GroundTruth {mDataManager.GetGroundTruth()};

    SQueries::tCrossingsInfo CrossingsInfo;

    const auto& Matches {mDataManager.GetMatches()};

    for (uint32_t MatchIndex {0U}; MatchIndex < Matches.oMatches.size(); MatchIndex++)
    {
        const auto& Route {Matches.oMatches[MatchIndex]};
        for (const auto& Edge : Route)
        {
            const auto              UnidirectionalEdgeIndex {GroundTruth.BidirectionalToUnidirectional(Edge)};
            SQueries::SCrossingInfo CrossingInfo {.MatchIndex = static_cast<int32_t>(MatchIndex)};
            CrossingsInfo.emplace(UnidirectionalEdgeIndex, std::move(CrossingInfo));
        }
    }

    // TODO: Remove this.
    // std::cout << "Printing crossing count:" << std::endl;
    // for (uint32_t TrackIndex {0U}; TrackIndex < GroundTruth.oNetwork.size(); TrackIndex++)
    //{
    //    std::cout << "[" << TrackIndex << "," << CrossingsInfo.count(TrackIndex) << "], ";
    //}
    // std::cout << std::endl;

    // TODO: Remove this.
    // Print crossings count historigram.
    std::map<size_t, s32> CrossingsCountHistogram;
    for (uint32_t TrackIndex {0U}; TrackIndex < GroundTruth.oNetwork.size(); TrackIndex++)
    {
        const size_t Count {CrossingsInfo.count(TrackIndex)};

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

    std::unique_ptr<SQueries> Queries = std::make_unique<SQueries>(std::move(CrossingsInfo));
    mDataManager.SetQueries(std::move(Queries));

    return Types::eLoadResult::Successful;
}
