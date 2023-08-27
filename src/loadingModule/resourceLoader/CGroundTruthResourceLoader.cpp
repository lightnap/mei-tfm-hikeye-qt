#include "CGroundTruthResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CGroundTruthResourceLoader>::Register(Types::eResource::GroundTruth)};
}

void CGroundTruthResourceLoader::LoadResource()
{
    // TODO: Fill this function.
}
