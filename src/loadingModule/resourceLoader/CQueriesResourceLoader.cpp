#include "CQueriesResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CQueriesResourceLoader>::Register(Types::eResource::Queries)};
}

Types::eLoadResult CQueriesResourceLoader::LoadResource()
{
    std::cout << "[QueriesResource] Loading queries" << std::endl;
    // TODO: Fill this function.

    return Types::eLoadResult::Successful;
}
