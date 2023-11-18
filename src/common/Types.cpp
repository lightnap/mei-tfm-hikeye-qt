#include "Types.hpp"
#include <cassert>

std::string Types::LoadingModuleToString(Types::eLoadingModule aModule)
{
    switch (aModule)
    {
        case Types::eLoadingModule::Terrain:
        {
            return "Terrain";
            break;
        }
        case Types::eLoadingModule::Tracks:
        {
            return "Tracks";
            break;
        }
        default:
        {
            break;
        }
    }

    assert(false && "Tried to convert to string an out of bounds eLoadingModue enum");
    return "";
}

std::string Types::ResourceToString(Types::eResource aResource)
{
    switch (aResource)
    {
        case Types::eResource::HeightMap:
        {
            return "HeightMap";
            break;
        }
        case Types::eResource::Terrain:
        {
            return "Terrain";
            break;
        }
        case Types::eResource::TerrainTexture:
        {
            return "TerrainTexture";
            break;
        }
        case Types::eResource::GroundTruth:
        {
            return "GroundTruth";
            break;
        }
        case Types::eResource::Matches:
        {
            return "Matches";
            break;
        }
        case Types::eResource::Queries:
        {
            return "Queries";
            break;
        }
        case Types::eResource::TracksTexture:
        {
            return "TracksTexture";
            break;
        }
        default:
        {
            break;
        }
    }

    assert(false && "Tried to convert to string an out of bounds eResource enum");
    return "";
}
