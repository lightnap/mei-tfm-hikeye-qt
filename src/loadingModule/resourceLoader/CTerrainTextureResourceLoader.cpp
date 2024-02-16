#include "CTerrainTextureResourceLoader.hpp"

#include "common/Types.hpp"
#include "dataStructures/CDataManager.hpp"
#include "dataStructures/STerrain.hpp"
#include "dataStructures/STexture.hpp"
#include "graphics/CCustomColourSpectrum.hpp"
#include "loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp"

#include <iostream>

namespace
{
    [[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTerrainTextureResourceLoader>::Register(Types::eResource::TerrainTexture)};
}

Types::eLoadResult CTerrainTextureResourceLoader::LoadResource()
{
    std::cout << "[TerrainTextureResource] Loading terrain texture" << std::endl;

    const auto& TerrainModel {mDataManager.GetTerrain()};
    const s32   TextureSizeX {static_cast<s32>(mDataManager.GetHeightMap().oResolution.oX)};
    const s32   TextureSizeY {static_cast<s32>(mDataManager.GetHeightMap().oResolution.oY)};

    const f64 MaxHeight {TerrainModel.oBounds.oMax.oZ};
    const f64 MinHeight {TerrainModel.oBounds.oMin.oZ};

    CCustomColourSpectrum ReliefColours {CCustomColourSpectrum::Relief()};

    Math::Vector3D LightDirection {Math::Normalize(Math::Vector3D(-1.0, -1.0, 2.5))};

    QImage TextureImage {TextureSizeX, TextureSizeY, QImage::Format::Format_RGBA8888};
    TextureImage.fill(QColor(255, 255, 0));

    for (s32 Column {0}; Column < TextureSizeX; Column++)
    {
        for (s32 Row {0}; Row < TextureSizeY; Row++)
        {
            // Terrain lighting.
            const f64            Height {TerrainModel.GetVertex(Row, Column).oZ};
            const f64            NormalizedHeight {Math::LinearStep(Height, MinHeight, MaxHeight)}; // TODO: HK-53 Why are we normalizingheight both inside and outside?
            const Math::Vector3D TerrainColour {ReliefColours.GetColor(NormalizedHeight)};
            const Math::Vector3D HeightLight {Math::Vector3D(1.0 * TerrainColour.oX, 0.7 * TerrainColour.oY, 0.5 * TerrainColour.oZ)};

            // Sun lighting. // TODO: HK-55 Clean this (and understand it). Also improve parameters so shading looks neater.
            f64 Light {Math::DotProduct(TerrainModel.GetNormal(Row, Column), LightDirection)};
            Light = 0.5 * (1.0 + Light);
            f64 cs {0.9 * Light};
            cs *= cs; // cosine like
            const Math::Vector3D SunLight {Math::Vector3D {cs}};

            // Ambient light.
            const Math::Vector3D Ambient {Math::Vector3D {0.5}};

            const Math::Vector3D PixelColor {(0.20 * Ambient + 0.40 * HeightLight + 0.40 * SunLight)};

            TextureImage.setPixelColor(Column, Row, PixelColor.ToQColor().rgb());
        }
    }

    std::unique_ptr<STexture> Texture {std::make_unique<STexture>(std::move(TextureImage))};
    mDataManager.SetTerrainTexture(std::move(Texture));

    return Types::eLoadResult::Successful;
}
