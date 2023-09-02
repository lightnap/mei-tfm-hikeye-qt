#include "CTerrainResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTerrainResourceLoader>::Register(Types::eResource::Terrain)};
}

Types::eResourceLoadingError CTerrainResourceLoader::LoadResource()
{
    std::cout << "[TerrainResource] Loading terrain model" << std::endl;
    // TODO: Fill this function.

    return Types::eResourceLoadingError::Successful;
}
