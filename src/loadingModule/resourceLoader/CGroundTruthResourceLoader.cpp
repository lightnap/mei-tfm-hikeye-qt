#include "CGroundTruthResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CGroundTruthResourceLoader>::Register(Types::eResource::GroundTruth)};
}

void CGroundTruthResourceLoader::LoadResource()
{
    std::cout << "[GroundTruthResource] Loading ground truth" << std::endl;
    // TODO: Fill this function.
}
