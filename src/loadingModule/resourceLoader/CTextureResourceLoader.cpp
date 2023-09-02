#include "CTextureResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTextureResourceLoader>::Register(Types::eResource::Texture)};
}

Types::eResourceLoadingError CTextureResourceLoader::LoadResource()
{
    std::cout << "[TextureResource] Loading texture" << std::endl;
    // TODO: Fill this function.

    return Types::eResourceLoadingError::Successful;
}
