#include "CGroundTruthResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: HK-25 Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CGroundTruthResourceLoader>::Register(Types::eResource::GroundTruth)};
}

Types::eLoadResult CGroundTruthResourceLoader::LoadResource()
{
    std::cout << "[GroundTruthResource] Loading ground truth" << std::endl;
    // TODO: HK-25 Fill this function.

    return Types::eLoadResult::Successful;
}
