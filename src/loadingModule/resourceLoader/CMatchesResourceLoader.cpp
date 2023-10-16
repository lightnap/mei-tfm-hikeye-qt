#include "CMatchesResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: HK-24 Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CMatchesResourceLoader>::Register(Types::eResource::Matches)};
}

Types::eLoadResult CMatchesResourceLoader::LoadResource()
{
    std::cout << "[MatchesResource] Loading matches" << std::endl;
    // TODO: HK-24 Fill this function.

    return Types::eLoadResult::Successful;
}
