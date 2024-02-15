#include "SGroundTruth.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <unordered_map>
#include <utility>

namespace
{
    struct STrackEndpoints
    {
        s64 FirstEndPoint {-1};
        s64 SecondEndPoint {-1};
    };

    bool operator==(const STrackEndpoints& aLhs, const STrackEndpoints& aRhs)
    {
        const bool EquallyOrdered {aLhs.FirstEndPoint == aRhs.FirstEndPoint && aLhs.SecondEndPoint == aRhs.SecondEndPoint};
        const bool ReverselyOrdered {aLhs.FirstEndPoint == aRhs.SecondEndPoint && aLhs.SecondEndPoint == aRhs.FirstEndPoint};
        return EquallyOrdered || ReverselyOrdered;
    }
}

SGroundTruth::SGroundTruth(const tNetwork&& aBidirectionalNetwork)
{

    // NOTE: We are assuming that having same beginning and endpoint means having the same track.
    // This works since right now our tracks are just segments, with two points.
    // But this could (will) fail if we ever have longer tracks.
    oNetwork.clear();

    std::unordered_map<size_t, STrackEndpoints> VisitedTrackEnpoints;
    for (size_t TrackIndex {0U}; TrackIndex < aBidirectionalNetwork.size(); TrackIndex++)
    {
        const auto&     Track {aBidirectionalNetwork.at(TrackIndex).oPoints};
        STrackEndpoints EndPoints {.FirstEndPoint = Track.front().oOsmId, .SecondEndPoint = Track.back().oOsmId};
        const auto&     VisitedTrackIt {
          std::find_if(VisitedTrackEnpoints.begin(), VisitedTrackEnpoints.end(), [&EndPoints](const auto& VisitedTrackIt) { return VisitedTrackIt.second == EndPoints; })};

        if (VisitedTrackIt != VisitedTrackEnpoints.end())
        {
            // Already seen this track, maybe in reverse order. Mark it as repeated.
            mBiToUniIndices[TrackIndex] = mBiToUniIndices[VisitedTrackIt->first];
        }
        else
        {
            // New track, repeated index will map to itself.
            mBiToUniIndices[TrackIndex] = oNetwork.size();
            oNetwork.push_back(aBidirectionalNetwork[TrackIndex]);
            VisitedTrackEnpoints[TrackIndex] = EndPoints;
        }
    }
}

s64 SGroundTruth::BidirectionalToUnidirectional(s64 aTrackIndex) const
{
    const auto& TrackIt {mBiToUniIndices.find(aTrackIndex)};
    if (TrackIt != mBiToUniIndices.end())
    {
        return TrackIt->second;
    }

    return aTrackIndex;
}
