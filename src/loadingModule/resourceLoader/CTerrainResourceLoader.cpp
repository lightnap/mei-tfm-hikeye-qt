#include "CTerrainResourceLoader.hpp"

#include "CConfigs.hpp"
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
    const auto& HeightMap {mDataManager.GetHeightMap()};
    std::cout << "[TerrainResource] Loading terrain model" << std::endl;

    STerrainConfig Config;
    Config.oCellSize = 2.0;       // TODO: HK-49 Turn this into config file (or read from tif).
    Config.oBounds.oMin.oX = 0.0; // 4633335.0; // TODO: HK-49 Turn this into config file (or read from tif).
    Config.oBounds.oMin.oY = 0.0; // 444825.0; // TODO: HK-49 Turn this into config file (or read from tif).
    Config.oBounds.oMax.oX = Config.oCellSize * HeightMap.oResolution.oX;
    Config.oBounds.oMax.oY = Config.oCellSize * HeightMap.oResolution.oY;

    auto Terrain {std::make_unique<STerrain>(HeightMap, Config)};
    mDataManager.SetTerrain(std::move(Terrain));

    if (isInterruptionRequested())
    {
        mLoadErrorMessage = "Interrupted by user";
        return Types::eLoadResult::Interrupted;
    }

    return Types::eLoadResult::Successful;
}
