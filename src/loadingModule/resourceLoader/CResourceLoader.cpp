#include "CResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

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

    const auto ErrorCode {static_cast<Types::eResourceLoadingError>(ReturnValue)};

    emit SendErrorCode(ErrorCode);
}

void CResourceLoader::LoadResourceSlot(Types::eResource aResource)
{
    if (mResourceType == aResource)
    {
        LoadResource();
    }

    quit();
}

void CResourceLoader::LoadResource()
{
    // TODO: Assert something here. This should not be called.
}
