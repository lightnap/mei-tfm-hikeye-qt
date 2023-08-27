#include "CQueriesResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CQueriesResourceLoader>::Register(Types::eResource::Queries)};
}

void CQueriesResourceLoader::LoadResource()
{
    // TODO: Fill this function.
}
