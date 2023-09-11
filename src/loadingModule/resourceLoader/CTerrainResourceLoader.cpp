#include "CTerrainResourceLoader.hpp"

#include "CHeightMap.hpp"
#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTerrainResourceLoader>::Register(Types::eResource::Terrain)};
}

Types::eLoadResult CTerrainResourceLoader::LoadResource()
{
    std::cout << "[TerrainResource] Loading terrain model" << std::endl;
    // TODO: Fill this function

    const auto& HeightMap {mDataManager.GetHeightMap()};

    // Create vector of vertices
    // Create vector of triangles
    // Create a structure containign both
    // Pass it to the data manager

    if (isInterruptionRequested())
    {
        return Types::eLoadResult::Interrupted;
    }

    return Types::eLoadResult::Successful;
}
