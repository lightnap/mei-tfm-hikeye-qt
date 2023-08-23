#include "Types.hpp"

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
    // TODO: Add an assert here.
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
        case Types::eResource::Texture:
        {
            return "Texture";
            break;
        }
        default:
        {
            break;
        }
    }
    // TODO: Add an assertion here.
    return "";
}
