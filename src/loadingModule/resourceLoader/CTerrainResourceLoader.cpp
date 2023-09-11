#include "CTerrainResourceLoader.hpp"

#include "CHeightMap.hpp"
#include "CResourceLoaderFactory.hpp"
#include "CTerrain.hpp"
#include "Types.hpp"

#include <iostream> //TODO: Remove this.
#include <memory>
#include <utility>

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTerrainResourceLoader>::Register(Types::eResource::Terrain)};
}

Types::eLoadResult CTerrainResourceLoader::LoadResource()
{
    std::cout << "[TerrainResource] Loading terrain model" << std::endl;

    const auto& HeightMap {mDataManager.GetHeightMap().GetHeights()};
    auto        Terrain {std::make_unique<CTerrain>(HeightMap)};
    mDataManager.SetTerrain(std::move(Terrain));

    if (isInterruptionRequested())
    {
        mLoadErrorMessage = "Interrupted by user";
        return Types::eLoadResult::Interrupted;
    }

    return Types::eLoadResult::Successful;
}
