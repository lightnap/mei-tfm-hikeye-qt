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
    const auto ReturnValue {LoadResource()};
    emit       ResourceLoadedSignal(static_cast<int>(ReturnValue));
    std::cout << "[RLoader] Finishing resource " << Types::ResourceToString(mResourceType) << std::endl;
}

Types::eResourceLoadingError CResourceLoader::LoadResource()
{
    // TODO: Assert something here. This should not be called.
    std::cout << "WARNING: Called the LoadResource of the base module" << std::endl;

    return Types::eResourceLoadingError::Size;
}
