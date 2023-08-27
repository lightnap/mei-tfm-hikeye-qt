#include "CTerrainResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTerrainResourceLoader>::Register(Types::eResource::Terrain)};
}

void CTerrainResourceLoader::LoadResource()
{
    // TODO: Fill this function.
}
