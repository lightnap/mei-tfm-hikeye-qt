#include "CTextureResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Types.hpp"

#include <iostream> //TODO: Remove this.

#include <CDataManager.hpp>
#include <SGroundTruth.hpp>

namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTextureResourceLoader>::Register(Types::eResource::Texture)};
}

Types::eLoadResult CTextureResourceLoader::LoadResource()
{
    std::cout << "[TextureResource] Loading texture" << std::endl;
    // TODO: Fill this function.

    const SGroundTruth& GroundTruth {mDataManager.GetGroundTruth()};
    std::cout << "[Texture] Got ground truth with how many tracks: " << GroundTruth.oNetwork.size() << std::endl;

    for (int i = 0; i < GroundTruth.oNetwork.size(); i++)
    {
        std::cout << "Track: " << i << " has size: " << GroundTruth.oNetwork.at(i).size() << std::endl;
        std::cout << "x= " << GroundTruth.oNetwork.at(i).at(0).oX << " y= " << GroundTruth.oNetwork.at(i).at(0).oY << std::endl;
    }

    return Types::eLoadResult::Successful;
}
