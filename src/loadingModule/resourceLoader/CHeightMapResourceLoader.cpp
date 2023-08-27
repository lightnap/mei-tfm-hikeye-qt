#include "CHeightMapResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CHeightMapResourceLoader>::Register(Types::eResource::HeightMap)};
}

void CHeightMapResourceLoader::LoadResource()
{
    // TODO: Fill this function.
}
