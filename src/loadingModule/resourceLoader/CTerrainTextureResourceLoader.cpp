#include "CTerrainTextureResourceLoader.hpp"

#include "common/Types.hpp"
#include "dataStructures/CDataManager.hpp"
#include "dataStructures/STerrain.hpp"
#include "dataStructures/STexture.hpp"
#include "loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp"

#include <iostream> // TODO: Remove this.

namespace
{
    [[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTerrainTextureResourceLoader>::Register(Types::eResource::TerrainTexture)};
}

Types::eLoadResult CTerrainTextureResourceLoader::LoadResource()
{
    // TODO: Remove this.
    std::cout << "[TerrainTextureResource] Loading terrain texture" << std::endl;

    const auto& TerrainModel {mDataManager.GetTerrain()};
    const s32   TextureSizeX {static_cast<s32>(mDataManager.GetHeightMap().oResolution.oX)};
    const s32   TextureSizeY {static_cast<s32>(mDataManager.GetHeightMap().oResolution.oY)};

    QImage TextureImage {TextureSizeX, TextureSizeY, QImage::Format::Format_RGBA8888};

    for (s32 Column {0}; Column < TextureSizeX; Column++)
    {
        for (s32 Row {0}; Row < TextureSizeY; Row++) {}
    }

    TextureImage.fill(QColor(255, 255, 0));

    std::unique_ptr<STexture> Texture {std::make_unique<STexture>(std::move(TextureImage))};
    mDataManager.SetTexture(std::move(Texture));

    return Types::eLoadResult::Successful;
}
