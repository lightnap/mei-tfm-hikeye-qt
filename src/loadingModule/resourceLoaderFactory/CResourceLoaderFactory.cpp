#include "CResourceLoaderFactory.hpp"

#include "common/Types.hpp"

#include <cassert>
#include <utility>

CResourceLoader* CResourceLoaderFactory::Create(Types::eResource aResource, CDataManager& aDataManager)
{
    auto FactoryIt {GetFactories().find(aResource)};
    assert(FactoryIt != GetFactories().cend() && "Error: Resource loader not registered to factory");
    return FactoryIt->second->MakeResourceLoader(aResource, aDataManager);
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
