#include "CResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CResourceLoader>::Register(Types::eResource::None)};
}

CResourceLoader::CResourceLoader(Types::eResource aResource)
  : mResourceType(aResource)
{
}

void CResourceLoader::run()
{
    u32 ReturnValue {0U};
    ReturnValue = exec();
    emit SendErrorCode(static_cast<int>(ReturnValue));
}

void CResourceLoader::LoadResourceSlot(Types::eResource aResource)
{
    if (mResourceType == aResource)
    {
        LoadResource();
        quit();
    }
}

void CResourceLoader::LoadResource()
{
    // TODO: Assert something here. This should not be called.
    std::cout << "WARNING: Called the LoadResource of the base module" << std::endl;
}
