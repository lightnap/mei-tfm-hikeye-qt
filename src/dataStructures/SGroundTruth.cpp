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

SGroundTruth::SGroundTruth(const tNetwork&& aNetwork)
  : oNetwork(std::move(aNetwork))
{

    // NOTE: We are assuming that having same beginning and endpoint means having the same track.
    // This works since right now our tracks are just segments, with two points.
    // But this could (will) fail if we ever have longer tracks.

    std::unordered_map<size_t, STrackEndpoints> VisitedTrackEnpoints;
    for (size_t TrackIndex {0U}; TrackIndex < aNetwork.size(); TrackIndex++)
    {
        const auto&     Track {aNetwork.at(TrackIndex).oPoints};
        STrackEndpoints EndPoints {.FirstEndPoint = Track.front().oOsmId, .SecondEndPoint = Track.back().oOsmId};
        const auto&     EndPointIt {
          std::find_if(VisitedTrackEnpoints.begin(), VisitedTrackEnpoints.end(), [&EndPoints](const auto& VisitedTrackIt) { return VisitedTrackIt.second == EndPoints; })};

        if (EndPointIt != VisitedTrackEnpoints.end())
        {
            // Already seen this track, maybe in reverse order. Mark it as repeated.
            mRepeatedTracks[TrackIndex] = EndPointIt->first;
        }
        else
        {
            // New track, repeated index will map to itself.
            mRepeatedTracks[TrackIndex] = TrackIndex;
            VisitedTrackEnpoints[TrackIndex] = EndPoints;
        }
    }
}

s64 SGroundTruth::BidirectionalToUnidirectional(s64 aTrackIndex) const
{
    const auto& TrackIt {mRepeatedTracks.find(aTrackIndex)};
    if (TrackIt != mRepeatedTracks.end())
    {
        return TrackIt->second;
    }

    return aTrackIndex;
}
