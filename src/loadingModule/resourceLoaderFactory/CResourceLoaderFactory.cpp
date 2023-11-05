#include "CResourceLoaderFactory.hpp"

#include "common/Types.hpp"

#include <utility>

CResourceLoader* CResourceLoaderFactory::Create(Types::eResource aResource, CDataManager& aDataManager)
{
    auto FactoryIt {GetFactories().find(aResource)};

    if (FactoryIt == GetFactories().cend())
    {
        // TODO: HK-34 Make an assert here.
    }

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
