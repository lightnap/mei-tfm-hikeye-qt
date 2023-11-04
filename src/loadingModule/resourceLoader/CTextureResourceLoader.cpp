#include "CTextureResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Math.hpp"
#include "Types.hpp"

#include "CDataManager.hpp"
#include "SGroundTruth.hpp"
#include "SHeightMap.hpp"
#include "STexture.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPen>

#include <iostream> //TODO: Remove this.
#include <memory>
#include <utility>
namespace
{
[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTextureResourceLoader>::Register(Types::eResource::Texture)};
}

Types::eLoadResult CTextureResourceLoader::LoadResource()
{
    // TODO: Clean this mess.
    std::cout << "[TextureResource] Loading texture" << std::endl;

    const s32 SizeX {static_cast<s32>(mDataManager.GetHeightMap().oResolution.oX)};
    const s32 SizeY {static_cast<s32>(mDataManager.GetHeightMap().oResolution.oY)};

    // TODO: HK-25: Paint paths on texture (right now it only paints it red).

    QImage TextureImage {SizeX, SizeY, QImage::Format::Format_RGBA8888};

    for (s32 PixelRow {0}; PixelRow < SizeX; PixelRow++)
    {
        for (s32 PixelColumn {0}; PixelColumn < SizeY; PixelColumn++)
        {
            TextureImage.setPixelColor(PixelRow, PixelColumn, QColor(255, 0, 0).toRgb());
        }
    }

    QPen     Pen {QColor(255, 255, 255), 10, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin};
    QPainter Painter {&TextureImage};

    const SGroundTruth& GroundTruth {mDataManager.GetGroundTruth()};

    for (const auto& Track : GroundTruth.oNetwork)
    {
        QPainterPath PathPainter;
        for (u32 PointIndex {0U}; PointIndex < Track.size(); PointIndex++)
        {
            // img coords
            int ii, jj;

            // TODO: Better constructors and take the 2 from the terrain resolution.
            Math::Vector2D Min(444825.0, 4633335.0 - 2 * TextureImage.height());
            Math::Vector2D Max(444825.0 + 2 * TextureImage.width(), 4633335.0);
            Math::Box2D    DomainBox;
            DomainBox.oMin = Min;
            DomainBox.oMax = Max;

            WorldToTexCoords(Track.at(PointIndex), DomainBox, TextureImage.size(), ii, jj);

            if (PointIndex == 0U)
            {
                PathPainter.moveTo(ii, jj);
            }
            else
            {
                PathPainter.lineTo(ii, jj);
            }
        }
        Painter.setPen(Pen);
        Painter.drawPath(PathPainter);
    }

    // TODO: Why are we mirroring this??
    QImage TextureImageMirror = std::move(TextureImage).mirrored(true, true);

    TextureImageMirror.save("/home/thedoa1013/code/hikeyeQt/data/matagalls/TrackTest.png");

    // TextureImage.load("/home/thedoa1013/code/hikeyeQt/data/matagalls/Untitled.jpeg"); // TOD:Delete this

    std::cout << "W= " << TextureImageMirror.width() << " H= " << TextureImageMirror.height() << std::endl;
    std::unique_ptr<STexture> Texture {std::make_unique<STexture>()};
    Texture->oTexture = std::move(TextureImageMirror);
    mDataManager.SetTexture(std::move(Texture));

    return Types::eLoadResult::Successful;
}

void CTextureResourceLoader::WorldToTexCoords(const Math::Vector2D& aWorldPoint, const Math::Box2D& aWorldBounds, const QSize& aTextureSize, s32& texi, s32& texj) const
{
    // TODO: Hungarian notation here.
    Math::Vector2D WorldDisplacement {aWorldPoint - aWorldBounds.oMin};
    Math::Vector2D WorldSize {aWorldBounds.oMax - aWorldBounds.oMin};

    f64 NormalizedX {WorldDisplacement.oX / WorldSize.oX};
    f64 NormalizedY {WorldDisplacement.oY / WorldSize.oY};

    texi = static_cast<s32>(NormalizedX * (aTextureSize.width() - 1));
    texj = static_cast<s32>(NormalizedY * (aTextureSize.height() - 1));
}
