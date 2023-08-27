#include "CMatchesResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CMatchesResourceLoader>::Register(Types::eResource::Matches)};
}

void CMatchesResourceLoader::LoadResource()
{
    // TODO: Fill this function.
}
