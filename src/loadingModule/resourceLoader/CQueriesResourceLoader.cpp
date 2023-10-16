#include "CQueriesResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: HK-24 Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CQueriesResourceLoader>::Register(Types::eResource::Queries)};
}

Types::eLoadResult CQueriesResourceLoader::LoadResource()
{
    std::cout << "[QueriesResource] Loading queries" << std::endl;
    // TODO: HK-24 Fill this function.

    return Types::eLoadResult::Successful;
}
