#include "CHeightMapResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> // TODO: Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CHeightMapResourceLoader>::Register(Types::eResource::HeightMap)};
}

void CHeightMapResourceLoader::LoadResource()
{
    std::cout << "[HeightMapResource] Loading height map" << std::endl;
    // TODO: Fill this function.
}
