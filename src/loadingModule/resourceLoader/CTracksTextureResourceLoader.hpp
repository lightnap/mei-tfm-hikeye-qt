#ifndef C_TRACKS_TEXTURE_RESOURCE_LOADER_H
#define C_TRACKS_TEXTURE_RESOURCE_LOADER_H

#include <QPen>

#include "common/Math.hpp"
#include "common/Types.hpp"
#include "loadingModule/resourceLoader/CResourceLoader.hpp"

/**
 * @brief Class that loads the texture that has the tracks information.
 */
class CTracksTextureResourceLoader : public CResourceLoader
{
    Q_OBJECT

  public:
    /**
     * @brief Inherited constructor.
     */
    using CResourceLoader::CResourceLoader;

  private:
    /**
     * @brief Loads the corresponding resource.
     * @return Code depicting the loading result.
     */
    Types::eLoadResult LoadResource() override;

    /**
     * @brief Draws ground truth over the provided image.
     * @param aImage: The image over which we want to draw the ground truth.
     */
    void DrawGroundTruth(QImage& aImage);

    /**
     * @brief Decides how much a track should be painted.
     * @param aTrackIndex: The track index of the param we want to paint.
     * @param aStrategy: Which strategy we are using to decide how to paing the track.
     * @return A numbe between 0 and 1. 0 means track is minimum painted, 1 means max.
     */
    f32 GetPaintingPercentage(u32 aTrackIndex, Types::ePaintStrategy aStrategy);

    /**
     * @brief Gets the pen that we should use to draw a given track.
     * @param aPercentage: Percentage (from 0 to 1) on how much should this track be painted.
     * @param aPaintingStrategy: Painting strategy indicates the palette.
     * @return The pen.
     */
    QPen GetPen(float aPercentage, Types::ePaintStrategy aPaintingStrategy);

    /**
     * @brief Turns world coordinates into row and column of a pixel in a texture.
     * @param aWorldPoint: World position (in utm) that we want to turn into a pixel position.
     * @param aWorldDomain: Rectangle of world that maps to the texture.
     * @param aTextureSize: Size of texture.
     * @return Vector containing row and column of pixel in image.
     */
    Math::Vector2D<s32> WorldToTexCoords(const Math::Vector2D<f64>& aWorldPoint, const Math::Box2D& aWorldBounds, const QSize& aTextueSize) const;

    bool HasArrow(s64 aTrackIndex, f32 PaintingPercentage, Types::ePaintStrategy ePaintingStrategy);
    void AddVectorialArrow(s64 aTrackIndex, Math::Vector2D<s32> aStartPoint, Math::Vector2D<s32> aEndPoint, f32 aPaintingPercentage);
    void DrawVectorialArrows(QPainter& aPainter);

    void AddImageArrow(s64 aTrackIndex, Math::Vector2D<s32> aStartPoint, Math::Vector2D<s32> aEndPoint, f32 aPaintingPercentage);
    void DrawImageArrows(QPainter& aPainter);

    std::unordered_map<s64, Types::eDirection> mPreferredDirections;   //!< For each track, in which direction do most people cross it.
    std::vector<QPolygonF>                     mVectorialArrows;       //!< We save the vectorial arrows to paint them all at once.
    std::vector<QTransform>                    mImageArrowsTransforms; //!< We save the image arrow transoform to paint them all at once.
    std::vector<QPointF>                       mImageArrowMiddle;      //
};

#endif // C_TRACKS_TEXTURE_RESOURCE_LOADER_H
