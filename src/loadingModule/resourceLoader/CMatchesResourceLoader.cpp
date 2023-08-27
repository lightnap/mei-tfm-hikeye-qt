#include "CMatchesResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CMatchesResourceLoader>::Register(Types::eResource::Matches)};
}

void CMatchesResourceLoader::LoadResource()
{
    std::cout << "[MatchesResource] Loading matches" << std::endl;
    // TODO: Fill this function.
}
