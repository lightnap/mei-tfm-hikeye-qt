#include "CHeightMapResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> // TODO: Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CHeightMapResourceLoader>::Register(Types::eResource::HeightMap)};
}

Types::eResourceLoadingError CHeightMapResourceLoader::LoadResource()
{
    std::cout << "[HeightMapResource] Loading height map" << std::endl;
    // TODO: Fill this function.
    for (int i = 0; i < 5; i++)
    {
        std::cout << "[Height] Doing work: " << i << std::endl;

        sleep(1);
        if (isInterruptionRequested())
        {
            return Types::eResourceLoadingError::UserInterruption;
        }
    }

    std::cout << "[Height] DataManager contains: " << mDataManager.GetFolderPath().toStdString() << std::endl;

    return Types::eResourceLoadingError::Successful;
}
