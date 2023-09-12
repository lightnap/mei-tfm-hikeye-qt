#include "CTerrainResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "SHeightMap.hpp"
#include "STerrain.hpp"
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

    const auto& HeightMap {mDataManager.GetHeightMap().oHeights};
    auto        Terrain {std::make_unique<STerrain>(HeightMap)};
    mDataManager.SetTerrain(std::move(Terrain));

    if (isInterruptionRequested())
    {
        mLoadErrorMessage = "Interrupted by user";
        return Types::eLoadResult::Interrupted;
    }

    return Types::eLoadResult::Successful;
}
