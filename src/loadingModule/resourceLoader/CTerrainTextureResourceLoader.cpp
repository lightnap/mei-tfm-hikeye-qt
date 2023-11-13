#include "CTerrainTextureResourceLoader.hpp"

#include "common/Types.hpp"
#include "dataStructures/CDataManager.hpp"
#include "dataStructures/STerrain.hpp"
#include "dataStructures/STexture.hpp"
#include "graphics/CCustomColourSpectrum.hpp"
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
            // const s32 VertexIndex {2 + 3 * (Column * TextureSizeY + Row)};
            // const f64 Height {TerrainModel.oVertices.at(VertexIndex)};

            const f64 Height {TerrainModel.GetVertex(Row, Column).oZ};

            // TODO: Clean this.
            const f64      t {Math::LinearStep(Height, MinHeight, MaxHeight)};
            Math::Vector3D cz {ReliefColours.GetColor(t)};

            // cz = 0.25 * cz;
            cz = Math::Vector3D(1 * cz.oX, 0.7 * cz.oY, 0.5 * cz.oZ);

            // Lighting.
            f64 cs {1.0};
            f64 Light {Math::DotProduct(TerrainModel.GetNormal(Row, Column), LightDirection)};
            Light = 0.5 * (1.0 + Light);
            // f64 cs {0.9 * Light};
            cs *= cs; // cosine like*/

            // Normal shading: color is a combination of cool and cold colors according to the orientation
            Math::Vector3D ambient = 0.25 * Math::Vector3D(1.0, 1.0, 1.0);
            Math::Vector3D c1 = 0.25 * cs * cz;
            Math::Vector3D c2 = 0.5 * cs * Math::Vector3D(1.0, 1.0, 1.0);
            Math::Vector3D c = ambient + c1 + c2;

            // std::cout << VertexIndex << std::endl;
            //  std::cout << "Height: " << Height << " Wavelength " << t << " cz.oX:" << cz.oX << std::endl;
            if (Column == 0)
            {
                std::cout << "Column: " << Column << " Row: " << Row << " MaxHeight: " << MaxHeight << " MinHeight" << MinHeight << std::endl;
                std::cout << "SizeX: " << TextureSizeX << " SizeY:  " << TextureSizeY << std::endl;
                std::cout << "Height: " << Height << " Wavelength " << t << " cz.oX:" << cz.oX << std::endl;
                std::cout << "C: " << c.oX << " " << c.oY << " " << c.oZ << std::endl;
                std::cout << std::endl;
            }

            // TODO: Why column row and not row column? Because we flipped the image?
            TextureImage.setPixelColor(Column, Row, c.ToQColor().rgb());
        }
    }

    std::unique_ptr<STexture> Texture {std::make_unique<STexture>(std::move(TextureImage))};
    mDataManager.SetTexture(std::move(Texture));

    return Types::eLoadResult::Successful;
}
