#include "CTextureResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTextureResourceLoader>::Register(Types::eResource::Texture)};
}

void CTextureResourceLoader::LoadResource()
{
    // TODO: Fill this function.
}
