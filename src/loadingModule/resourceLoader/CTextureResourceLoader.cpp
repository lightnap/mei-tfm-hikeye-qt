#include "CTextureResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: Remove this.

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTextureResourceLoader>::Register(Types::eResource::Texture)};
}

void CTextureResourceLoader::LoadResource()
{
    std::cout << "[TextureResource] Loading texture" << std::endl;
    // TODO: Fill this function.
}
