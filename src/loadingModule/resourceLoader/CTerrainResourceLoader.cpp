#include "CTerrainResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTerrainResourceLoader>::Register(Types::eResource::Terrain)};
}

void CTerrainResourceLoader::LoadResource()
{
    std::cout << "[HeightMapResource] Loading height map" << std::endl;
    // TODO: Fill this function.
}
