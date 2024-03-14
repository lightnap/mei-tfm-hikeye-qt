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
#include <cmath>
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
    QImage TextureImage {mDataManager.GetTerrainTexture().oTexture.mirrored(true, true)};

    DrawGroundTruth(TextureImage);

    // TODO: HK-54 Why are we mirroring this??
    // TextureImage.mirrored(false, true).save(SaveToFilePath);

    // TODO: HK-54 Why are we mirroring this??
    QImage TextureImageMirror = std::move(TextureImage).mirrored(true, true);

    std::unique_ptr<STexture> Texture {std::make_unique<STexture>(std::move(TextureImageMirror))};
    mDataManager.SetTracksTexture(std::move(Texture));

    return Types::eLoadResult::Successful;
}

void CTracksTextureResourceLoader::DrawGroundTruth(QImage& aImage)
{
    QPainter Painter {&aImage};

    Types::ePaintStrategy PaintStrategy {mDataManager.GetPaintStrategy()};
    const SGroundTruth&   GroundTruth {mDataManager.GetGroundTruth()};

    std::cout << "Image size is " << aImage.size().height() << " , " << aImage.size().width() << std::endl;

    for (u32 TrackIndex {0U}; TrackIndex < GroundTruth.oNetwork.size(); TrackIndex++)
    {
        const auto&  Track {GroundTruth.oNetwork.at(TrackIndex)};
        const auto   PaintingPercentage {GetPaintingPercentage(TrackIndex, PaintStrategy)};
        QPen         Pen {GetPen(PaintingPercentage, PaintStrategy)};
        QPainterPath PathPainter;

        // TODO: HK-52 Take min and max values from the terrain resolution.
        // So avoid being harcoded.
        const Math::Vector2D<f64> Min(444825.0, 4633335.0 - 2 * aImage.height());
        const Math::Vector2D<f64> Max(444825.0 + 2 * aImage.width(), 4633335.0);
        const Math::Box2D         DomainBox {Min, Max};

        const auto& StartPoint {Track.oPoints.front()};
        const auto& EndPoint {Track.oPoints.back()};

        Math::Vector2D<s32> StartPointTextureCoordinates {WorldToTexCoords(Math::Vector2D {StartPoint.oEasting, StartPoint.oNorthing}, DomainBox, aImage.size())};
        Math::Vector2D<s32> EndPointTextureCoordinates {WorldToTexCoords(Math::Vector2D {EndPoint.oEasting, EndPoint.oNorthing}, DomainBox, aImage.size())};

        PathPainter.moveTo(StartPointTextureCoordinates.oX, StartPointTextureCoordinates.oY);
        PathPainter.lineTo(EndPointTextureCoordinates.oX, EndPointTextureCoordinates.oY);

        Painter.setPen(Pen);
        Painter.drawPath(PathPainter);

        if (HasArrow(TrackIndex, PaintingPercentage, PaintStrategy))
        {
            DrawArrow(TrackIndex, Painter, StartPointTextureCoordinates, EndPointTextureCoordinates, DomainBox, aImage.size());
        }
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
        case Types::ePaintStrategy::Directions:
        {
            const auto& Queries {mDataManager.GetQueries()};
            const auto& [LowerElementIt, HigherElementIt] {Queries.oCrossingsInfo.equal_range(aTrackIndex)};

            s32 DirectionalHits {0};

            std::for_each(LowerElementIt,
                          HigherElementIt,
                          [&DirectionalHits](const auto& CrossingInfoIt)
                          { CrossingInfoIt.second.Direction == Types::eDirection::Positive ? DirectionalHits++ : DirectionalHits--; });

            if (DirectionalHits > 0)
            {
                mPreferredDirections[aTrackIndex] = Types::eDirection::Positive;
            }
            else if (DirectionalHits < 0)
            {
                mPreferredDirections[aTrackIndex] = Types::eDirection::Negative;
            }
            else
            {
                mPreferredDirections[aTrackIndex] = Types::eDirection::None;
            }

            static constexpr s32 MAX_DIRECTIONAL_HITS {10};
            const f32            DirectionPercentage {std::min(1.0f, static_cast<f32>(std::abs(DirectionalHits)) / static_cast<f32>(MAX_DIRECTIONAL_HITS))};

            return DirectionPercentage;
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

QPen CTracksTextureResourceLoader::GetPen(float aPercentage, Types::ePaintStrategy aPaintingStrategy)
{
    Math::Vector3D Color;

    switch (aPaintingStrategy)
    {
        case Types::ePaintStrategy::CountCrossings:
        case Types::ePaintStrategy::CountCrossingsPerMatch:
        case Types::ePaintStrategy::Speed:
        case Types::ePaintStrategy::None:
        {
            Color = CCustomColourSpectrum::CoolWarm().GetColor(aPercentage);
            break;
        }
        case Types::ePaintStrategy::Directions:
        {

            Color = CCustomColourSpectrum::Symmetric().GetColor(aPercentage);
            break;
        }
        defaullt:
        {
            Color = Math::Vector3D(0, 0, 0);
            break;
        }
    }

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

bool CTracksTextureResourceLoader::HasArrow(s64 aTrackIndex, f32 PaintingPercentage, Types::ePaintStrategy ePaintingStrategy)
{
    if (ePaintingStrategy != Types::ePaintStrategy::Directions)
    {
        return false;
    }
    else if (PaintingPercentage < 0.1f)
    {
        return false;
    }
    else if (aTrackIndex % 200 != 0)
    {
        return false;
    }

    return true;
}

void CTracksTextureResourceLoader::DrawArrow(s64                 aTrackIndex,
                                             QPainter&           aPainter,
                                             Math::Vector2D<s32> aStartPoint,
                                             Math::Vector2D<s32> aEndPoint,
                                             const Math::Box2D&  aWorldBounds,
                                             const QSize&        aTextureSize)
{
    Math::Vector2D<double> TrackMiddle {0.5 * (aStartPoint.oX + aEndPoint.oX), 0.5 * (aStartPoint.oY + aEndPoint.oY)};

    Math::Vector2D<double> ArrowDirection {(aEndPoint - aStartPoint).Normalize()};

    static constexpr f64 ARROW_MAX_HEIGHT {30.};
    static constexpr f64 ARROW_MAX_WIDTH {15.};

    f64 ArrowTipOffset {mPreferredDirections[aTrackIndex] == Types::eDirection::Positive ? ARROW_MAX_HEIGHT / 2.0 : -ARROW_MAX_HEIGHT / 2.0};

    const Math::Vector2D<double> ArrowTip {TrackMiddle + ArrowTipOffset * ArrowDirection};

    const Math::Vector2D<double> ArrowBottomCenter {TrackMiddle - ArrowTipOffset * ArrowDirection};

    const Math::Vector2D<double> ArrowPerpendicular {-ArrowDirection.oY, ArrowDirection.oX};
    const Math::Vector2D<double> ArrowRightPoint {ArrowBottomCenter + ARROW_MAX_WIDTH / 2.0 * ArrowPerpendicular};
    const Math::Vector2D<double> ArrowLeftPoint {ArrowBottomCenter - ARROW_MAX_WIDTH / 2.0 * ArrowPerpendicular};

    const QPointF ArrowTipQPoint {ArrowTip.oX, ArrowTip.oY};
    const QPointF ArrowLeftQPoint {ArrowLeftPoint.oX, ArrowLeftPoint.oY};
    const QPointF ArrowRightQPoint {ArrowRightPoint.oX, ArrowRightPoint.oY};

    std::cout << "Printing an arrow with tip " << ArrowTip.oX << "," << ArrowTip.oY << " left= " << ArrowLeftPoint.oX << "," << ArrowLeftPoint.oY << " right " << ArrowRightPoint.oX
              << "," << ArrowRightPoint.oY << std::endl;

    QPolygonF Arrow;
    Arrow << ArrowTipQPoint << ArrowLeftQPoint << ArrowRightQPoint;
    QPen   Pen {aPainter.pen()};
    QBrush Brush {Pen.brush()};
    Brush.setStyle(Qt::BrushStyle::SolidPattern);
    Pen.setBrush(Brush);
    Pen.setCapStyle(Qt::PenCapStyle::SquareCap);
    Pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
    Pen.setColor(Qt::black);
    aPainter.setPen(Pen);
    aPainter.drawPolygon(Arrow);
}
