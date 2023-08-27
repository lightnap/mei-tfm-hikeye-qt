#include "CResourceLoaderFactory.hpp"

#include "Types.hpp"

#include <utility>

CResourceLoader* CResourceLoaderFactory::Create(Types::eResource aResource)
{
    auto FactoryIt {GetFactories().find(aResource)};

    if (FactoryIt == GetFactories().cend())
    {
        // TODO: Make an assert here.
    }

    return FactoryIt->second->MakeResourceLoader(aResource);
}

bool CResourceLoaderFactory::Register(Types::eResource aResourceType, tConcreteFactoryUnique apResourceLoaderFactory)
{
    auto Result {GetFactories().emplace(aResourceType, std::move(apResourceLoaderFactory))};
    return Result.second;
}

CResourceLoaderFactory::tResourceLoaderFactories& CResourceLoaderFactory::GetFactories()
{
    static tResourceLoaderFactories Factories {};
    return Factories;
}
