#include "CTracksTextureResourceLoader.hpp"

#include "common/Math.hpp"
#include "common/Types.hpp"
#include "loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp"

#include "dataStructures/CDataManager.hpp"
#include "dataStructures/SGroundTruth.hpp"
#include "dataStructures/SHeightMap.hpp"
#include "dataStructures/STexture.hpp"
#include "graphics/CCustomColourSpectrum.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPen>

#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
#include <utility>

namespace
{

    [[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTracksTextureResourceLoader>::Register(Types::eResource::TracksTexture)};
}

Types::eLoadResult CTracksTextureResourceLoader::LoadResource()
{
    std::cout << "[TracksTextureResource] Loading tracks texture" << std::endl;

    // TODO: HK-54 Why are we mirroring this??
    QImage TextureImage {mDataManager.GetTexture().oTexture.mirrored(true, true)};

    DrawGroundTruth(TextureImage);

    // TODO: HK-54 Why are we mirroring this??
    // TextureImage.mirrored(false, true).save(SaveToFilePath);

    // TODO: HK-54 Why are we mirroring this??
    QImage TextureImageMirror = std::move(TextureImage).mirrored(true, true);

    std::unique_ptr<STexture> Texture {std::make_unique<STexture>(std::move(TextureImageMirror))};
    mDataManager.SetTexture(std::move(Texture));

    return Types::eLoadResult::Successful;
}

void CTracksTextureResourceLoader::DrawGroundTruth(QImage& aImage)
{
    QPainter Painter {&aImage};

    Types::ePaintStrategy PaintStrategy {mDataManager.GetPaintStrategy()};
    const SGroundTruth&   GroundTruth {mDataManager.GetGroundTruth()};

    for (u32 TrackIndex {0U}; TrackIndex < GroundTruth.oNetwork.size(); TrackIndex++)
    {
        const auto&  Track {GroundTruth.oNetwork.at(TrackIndex)};
        const auto   PaintingPercentage {GetPaintingPercentage(TrackIndex, PaintStrategy)};
        QPen         Pen {GetPen(PaintingPercentage)};
        QPainterPath PathPainter;

        for (u32 PointIndex {0U}; PointIndex < Track.oPoints.size(); PointIndex++)
        {
            // TODO: HK-52 Take min and max values from the terrain resolution.
            // So avoid being harcoded.
            Math::Vector2D<f64> Min(444825.0, 4633335.0 - 2 * aImage.height());
            Math::Vector2D<f64> Max(444825.0 + 2 * aImage.width(), 4633335.0);
            Math::Box2D         DomainBox {Min, Max};

            const auto&         Point {Track.oPoints.at(PointIndex)};
            Math::Vector2D<s32> TextureCoordinates {WorldToTexCoords(Math::Vector2D {Point.oEasting, Point.oNorthing}, DomainBox, aImage.size())};

            if (PointIndex == 0U)
            {
                PathPainter.moveTo(TextureCoordinates.oX, TextureCoordinates.oY);
            }
            else
            {
                PathPainter.lineTo(TextureCoordinates.oX, TextureCoordinates.oY);
            }
        }
        Painter.setPen(Pen);
        Painter.drawPath(PathPainter);
    }
}

f32 CTracksTextureResourceLoader::GetPaintingPercentage(u32 aTrackIndex, Types::ePaintStrategy aStrategy)
{
    switch (aStrategy)
    {
        case Types::ePaintStrategy::None:
        {
            return 0.0f;
            break;
        }
        case Types::ePaintStrategy::CountCrossings:
        {
            static constexpr s32 MAX_CROSSINGS {30};

            const auto& Queries {mDataManager.GetQueries()};
            const auto  CrossingCount {Queries.oCrossingsInfo.count(aTrackIndex)};
            const f32   CrossingsPercentage {std::min(1.0f, static_cast<f32>(CrossingCount) / static_cast<f32>(MAX_CROSSINGS))};

            return CrossingsPercentage;
            break;
        }
        case Types::ePaintStrategy::CountCrossingsPerMatch:
        {
            const auto& Queries {mDataManager.GetQueries()};
            const auto& [LowerElementIt, HigherElementIt] {Queries.oCrossingsInfo.equal_range(aTrackIndex)};

            std::set<u32> MatchesIdsOnTrack {};

            std::for_each(LowerElementIt, HigherElementIt, [&MatchesIdsOnTrack](const auto& CrossingInfoIt) { MatchesIdsOnTrack.insert(CrossingInfoIt.second.MatchIndex); });

            static constexpr s32 MAX_NON_REPEATING_CROSSINGS {30};
            const f32            CrossingsPercentage {std::min(1.0f, static_cast<f32>(MatchesIdsOnTrack.size()) / static_cast<f32>(MAX_NON_REPEATING_CROSSINGS))};

            return CrossingsPercentage;
            break;
        }
        default:
        {
            return 0.0f;
            break;
        }
    }

    return 0.0f;
}

QPen CTracksTextureResourceLoader::GetPen(float aPercentage)
{
    auto Color {CCustomColourSpectrum::CoolWarm().GetColor(aPercentage)};

    static constexpr f32 MAX_BRUSH_WIDTH {20.0f};
    static constexpr f32 MIN_BRUSH_WIDTH {5.0f};
    const f64            BrushWidth {Math::Lerp(MIN_BRUSH_WIDTH, MAX_BRUSH_WIDTH, aPercentage)};

    QPen Pen {Color.ToQColor(), BrushWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    return Pen;
}

Math::Vector2D<s32> CTracksTextureResourceLoader::WorldToTexCoords(const Math::Vector2D<f64>& aWorldPoint, const Math::Box2D& aWorldBounds, const QSize& aTextureSize) const
{
    Math::Vector2D<f64> WorldDisplacement {aWorldPoint - aWorldBounds.oMin};
    Math::Vector2D<f64> WorldSize {aWorldBounds.oMax - aWorldBounds.oMin};

    f64 NormalizedX {WorldDisplacement.oX / WorldSize.oX};
    f64 NormalizedY {WorldDisplacement.oY / WorldSize.oY};

    s32 PixelColumn {static_cast<s32>(NormalizedX * (aTextureSize.width() - 1))};
    s32 PixelRow {static_cast<s32>(NormalizedY * (aTextureSize.height() - 1))};

    return {PixelColumn, PixelRow};
}
