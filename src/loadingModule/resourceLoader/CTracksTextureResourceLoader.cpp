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
#include <QtMath>

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
    mVectorialArrows.clear();
    mImageArrowsTransforms.clear();
    mPaintValues.clear();
    mMinPaintableValue = 0;
    mMaxPaintableValue = 0;

    QPainter Painter {&aImage};

    std::cout << "Image size is " << aImage.size().height() << " , " << aImage.size().width() << std::endl;
    ComputePaintingValues();

    // TODO: HK-52 Take min and max values from the terrain resolution.
    // So avoid being harcoded.
    const Math::Vector2D<f64> Min(444825.0, 4633335.0 - 2 * aImage.height());
    const Math::Vector2D<f64> Max(444825.0 + 2 * aImage.width(), 4633335.0);
    const Math::Box2D         DomainBox {Min, Max};

    Types::ePaintStrategy PaintStrategy {mDataManager.GetPaintStrategy()};
    const SGroundTruth&   GroundTruth {mDataManager.GetGroundTruth()};

    for (u32 TrackIndex {0U}; TrackIndex < GroundTruth.oNetwork.size(); TrackIndex++)
    {
        const auto&  Track {GroundTruth.oNetwork.at(TrackIndex)};
        const f32    PaintingPercentage {GetPaintingPercentage(TrackIndex, PaintStrategy)};
        QPen         Pen {GetPen(PaintingPercentage, PaintStrategy)};
        QPainterPath PathPainter;

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
            // AddVectorialArrow(TrackIndex, StartPointTextureCoordinates, EndPointTextureCoordinates, PaintingPercentage);
            AddImageArrow(TrackIndex, StartPointTextureCoordinates, EndPointTextureCoordinates, PaintingPercentage);
        }
    }

    // DrawVectorialArrows(Painter);
    DrawImageArrows(Painter);
    mVectorialArrows.clear();
    mImageArrowsTransforms.clear();
}

void CTracksTextureResourceLoader::ComputePaintingValues()
{
    mMinPaintableValue = 0;
    mMaxPaintableValue = 0;

    Types::ePaintStrategy PaintStrategy {mDataManager.GetPaintStrategy()};
    const SGroundTruth&   GroundTruth {mDataManager.GetGroundTruth()};

    if (PaintStrategy == Types::ePaintStrategy::None)
    {
        return;
    }

    for (u32 TrackIndex {0U}; TrackIndex < GroundTruth.oNetwork.size(); TrackIndex++)
    {
        const auto& Track {GroundTruth.oNetwork.at(TrackIndex)};
        const s32   PaintingValue {GetPaintingValue(TrackIndex, PaintStrategy)};
        mPaintValues.push_back(PaintingValue);
        mMinPaintableValue = std::min(PaintingValue, mMinPaintableValue);
        mMaxPaintableValue = std::max(PaintingValue, mMaxPaintableValue);
    }

    mDataManager.SetPaintRangeMax(mMaxPaintableValue);
    mDataManager.SetPaintRangeMin(mMinPaintableValue);

    // DEBUG:
    // Print histogram.
    std::map<s32, s32> HistogramMap;

    for (u32 TrackIndex {0U}; TrackIndex < GroundTruth.oNetwork.size(); TrackIndex++)
    {
        const s32 Value {mPaintValues[TrackIndex]};

        const auto HistogramIt {HistogramMap.find(Value)};
        if (HistogramIt == HistogramMap.end())
        {
            HistogramMap[Value] = 1;
        }
        else
        {
            HistogramIt->second++;
        }
    }

    std::cout << "Printing Visualization Hisogram:" << std::endl;

    for (const auto& [Value, TimesItHappened] : HistogramMap)
    {
        std::cout << "A segment painted with value: " << Value << " was found: " << TimesItHappened << " times" << std::endl;
        // std::cout << Value << std::endl;
        // std::cout << TimesItHappened << std::endl;
    }
}

s32 CTracksTextureResourceLoader::GetPaintingValue(u32 aTrackIndex, Types::ePaintStrategy aStrategy)
{
    switch (aStrategy)
    {
        case Types::ePaintStrategy::CountCrossings:
        {
            const s32 MaxCrossings {mDataManager.GetPaintRangeCurrentUpper()};
            const s32 MinCrossings {mDataManager.GetPaintRangeCurrentLower()};

            const auto& Queries {mDataManager.GetQueries()};
            const s32   CrossingCount {static_cast<s32>(Queries.oCrossingsInfo.count(aTrackIndex))};

            return CrossingCount;
            break;
        }
        case Types::ePaintStrategy::CountCrossingsPerMatch:
        {
            const s32 MaxCrossings {mDataManager.GetPaintRangeCurrentUpper()};
            const s32 MinCrossings {mDataManager.GetPaintRangeCurrentLower()};

            const auto& Queries {mDataManager.GetQueries()};
            const auto& [LowerElementIt, HigherElementIt] {Queries.oCrossingsInfo.equal_range(aTrackIndex)};

            std::set<u32> MatchesIdsOnTrack {};
            std::for_each(LowerElementIt, HigherElementIt, [&MatchesIdsOnTrack](const auto& CrossingInfoIt) { MatchesIdsOnTrack.insert(CrossingInfoIt.second.MatchIndex); });
            const s32 NonRepeatingCrossingCount {static_cast<s32>(MatchesIdsOnTrack.size())};

            return NonRepeatingCrossingCount;
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

            return std::abs(DirectionalHits);
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

f32 CTracksTextureResourceLoader::GetPaintingPercentage(u32 aTrackIndex, Types::ePaintStrategy aStrategy)
{
    // TODO THIS:
    if (aStrategy == Types::ePaintStrategy::None)
    {
        return 0.0f;
    }

    const s32 MaxCrossings {mDataManager.GetPaintRangeCurrentUpper()};
    const s32 MinCrossings {mDataManager.GetPaintRangeCurrentLower()};

    switch (aStrategy)
    {
        case Types::ePaintStrategy::CountCrossings:
        case Types::ePaintStrategy::CountCrossingsPerMatch:
        case Types::ePaintStrategy::Directions:
        {
            f32 Percentage {static_cast<f32>(mPaintValues[aTrackIndex] - MinCrossings) / static_cast<f32>(MaxCrossings - MinCrossings)};
            Percentage = std::clamp(Percentage, 0.0f, 1.0f);
            return Percentage;
        }

        default:
        {
            return 0.0f;
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
        default:
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
    else if (PaintingPercentage <= 0.2f)
    {
        return false;
    }
    else if (aTrackIndex % 50 != 0)
    {
        return false;
    }

    return true;
}

void CTracksTextureResourceLoader::AddVectorialArrow(s64 aTrackIndex, Math::Vector2D<s32> aStartPoint, Math::Vector2D<s32> aEndPoint, f32 aPaintingPercentage)
{
    Math::Vector2D<double> TrackMiddle {0.5 * (aStartPoint.oX + aEndPoint.oX), 0.5 * (aStartPoint.oY + aEndPoint.oY)};

    Math::Vector2D<double> ArrowDirection {(aEndPoint - aStartPoint).Normalize()};

    static constexpr f64 ARROW_MAX_HEIGHT {70.};
    static constexpr f64 ARROW_MAX_WIDTH {20.};

    f64 ArrowTipOffset {mPreferredDirections[aTrackIndex] == Types::eDirection::Positive ? ARROW_MAX_HEIGHT * aPaintingPercentage / 2.0
                                                                                         : -ARROW_MAX_HEIGHT * aPaintingPercentage / 2.0};

    const Math::Vector2D<double> ArrowTip {TrackMiddle + ArrowTipOffset * ArrowDirection};

    const Math::Vector2D<double> ArrowBottomCenter {TrackMiddle - ArrowTipOffset * ArrowDirection};

    const Math::Vector2D<double> ArrowPerpendicular {-ArrowDirection.oY, ArrowDirection.oX};
    const Math::Vector2D<double> ArrowRightPoint {ArrowBottomCenter + ARROW_MAX_WIDTH / 2.0 * ArrowPerpendicular};
    const Math::Vector2D<double> ArrowLeftPoint {ArrowBottomCenter - ARROW_MAX_WIDTH / 2.0 * ArrowPerpendicular};

    const QPointF ArrowTipQPoint {ArrowTip.oX, ArrowTip.oY};
    const QPointF ArrowLeftQPoint {ArrowLeftPoint.oX, ArrowLeftPoint.oY};
    const QPointF ArrowRightQPoint {ArrowRightPoint.oX, ArrowRightPoint.oY};

    // std::cout << "Adding a vector arrow: "
    //           << "StartPoint: " << aStartPoint.oX << "," << aStartPoint.oY << " endPoint: " << aEndPoint.oX << "," << aEndPoint.oY << " middle point " << TrackMiddle.oX <<
    //           ","
    //           << TrackMiddle.oY << " tip " << ArrowTip.oX << "," << ArrowTip.oY << " left= " << ArrowLeftPoint.oX << "," << ArrowLeftPoint.oY << " right " <<
    //           ArrowRightPoint.oX
    //           << "," << ArrowRightPoint.oY << std::endl;

    QPolygonF Arrow;
    Arrow << ArrowTipQPoint << ArrowLeftQPoint << ArrowRightQPoint;
    mVectorialArrows.emplace_back(std::move(Arrow));
}

void CTracksTextureResourceLoader::DrawVectorialArrows(QPainter& aPainter)
{
    QPen   Pen {aPainter.pen()};
    QBrush Brush {Pen.brush()};
    Brush.setStyle(Qt::BrushStyle::SolidPattern);
    Pen.setBrush(Brush);
    Pen.setCapStyle(Qt::PenCapStyle::SquareCap);
    Pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
    Pen.setColor(Qt::black);
    aPainter.setPen(Pen);

    for (const auto& Arrow : mVectorialArrows)
    {
        aPainter.drawPolygon(Arrow);
    }
}

void CTracksTextureResourceLoader::AddImageArrow(s64 aTrackIndex, Math::Vector2D<s32> aStartPoint, Math::Vector2D<s32> aEndPoint, f32 aPaintingPercentage)
{
    static constexpr double      IMAGE_SCALING {0.1};
    const Math::Vector2D<double> ArrowMiddle {0.5 * (aStartPoint.oX + aEndPoint.oX), 0.5 * (aStartPoint.oY + aEndPoint.oY)};
    const Math::Vector2D<double> ArrowVector {static_cast<double>(aEndPoint.oX - aStartPoint.oX), static_cast<double>(aEndPoint.oY - aStartPoint.oY)};
    double                       ArrowRotation {Math::RadToDeg(qAtan2(ArrowVector.oY, ArrowVector.oX))};

    if (mPreferredDirections[aTrackIndex] == Types::eDirection::Negative)
    {
        ArrowRotation += 180.0;
    }

    const double ArrowVerticalScaling {static_cast<double>(aPaintingPercentage) + 1.0};

    QTransform Transform;
    Transform.rotate(ArrowRotation);
    Transform.scale(IMAGE_SCALING, IMAGE_SCALING);
    Transform.scale(ArrowVerticalScaling, 1.0);

    mImageArrowMiddle.emplace_back(ArrowMiddle.oX, ArrowMiddle.oY);
    mImageArrowsTransforms.emplace_back(std::move(Transform));
}

void CTracksTextureResourceLoader::DrawImageArrows(QPainter& aPainter)
{
    QPen   Pen {aPainter.pen()};
    QBrush Brush {Pen.brush()};
    Brush.setStyle(Qt::BrushStyle::SolidPattern);
    Pen.setBrush(Brush);
    Pen.setCapStyle(Qt::PenCapStyle::SquareCap);
    Pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
    Pen.setColor(Qt::black);
    aPainter.setPen(Pen);
    QImage Arrow {"../data/icons/arrow.png"};

    for (s32 Index {0}; Index < mImageArrowsTransforms.size(); Index++)
    {
        const auto    TransformedArrow {Arrow.transformed(mImageArrowsTransforms[Index])};
        const QPointF ArrowPosition {mImageArrowMiddle[Index].x() - TransformedArrow.width() / 2.0, mImageArrowMiddle[Index].y() - TransformedArrow.height() / 2.0};
        aPainter.drawImage(ArrowPosition, TransformedArrow);
    }
}
