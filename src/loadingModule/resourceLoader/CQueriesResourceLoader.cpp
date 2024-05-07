#include "CQueriesResourceLoader.hpp"

#include "QDate"

#include "common/Types.hpp"
#include "dataStructures/SQueries.hpp"
#include "loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <time.h>
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
        const auto& Match {Matches.oMatches[MatchIndex]};

        if (MustFilter(Match))
        {
            continue;
        }

        for (const auto& Edge : Match.EdgeIndices)
        {
            const auto              UnidirectionalEdgeIndex {GroundTruth.BidirectionalToUnidirectional(Edge)};
            const Types::eDirection Direction {GroundTruth.GetDirection(Edge)};
            SQueries::SCrossingInfo CrossingInfo {.MatchIndex = static_cast<int32_t>(MatchIndex), .Direction = Direction};
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
    /*std::map<size_t, s32> CrossingsCountHistogram;
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
      }/**/

    std::unique_ptr<SQueries> Queries = std::make_unique<SQueries>(std::move(CrossingsInfo));
    mDataManager.SetQueries(std::move(Queries));

    return Types::eLoadResult::Successful;
}

bool CQueriesResourceLoader::MustFilter(const SMatches::SMatch& aMatch) const
{
    Types::sDateFilter Filter {mDataManager.GetDateFilter()};

    time_t DateInEpoch {aMatch.Date};
    tm     DateInLocal = *localtime(&DateInEpoch);
    QDate  Date(DateInLocal.tm_year, DateInLocal.tm_mon, DateInLocal.tm_mday);

    const bool ValidDay {!Filter.FilteredDays[Date.dayOfWeek()]};
    const bool ValidMonth {!Filter.FilteredMonths[Date.month()]};

    return !ValidDay || !ValidMonth;
}
