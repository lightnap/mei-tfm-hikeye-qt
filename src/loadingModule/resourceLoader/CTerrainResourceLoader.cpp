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

    for (int i = 0; i < 10; i++)
    {
        std::cout << "[Terrain] Doing work: " << i << std::endl;

        sleep(1);
        if (isInterruptionRequested())
        {
            return Types::eResourceLoadingError::UserInterruption;
        }
    }

    return Types::eResourceLoadingError::Successful;
}
