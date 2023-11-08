#include "SHeightMap.hpp"

#include "common/Math.hpp"
#include "common/Types.hpp"
#include "dataStructures/CConfigs.hpp"

#include <QImage>

SHeightMap::SHeightMap(QImage& aTexture, const SHeightMapConfig& aConfig)
  : oMaxHeight(aConfig.oMaxHeight)
  , oMinHeight(aConfig.oMinHeight)
  , oResolution(0U, 0U)
{
    oHeights.clear();

    const u32 SizeX {static_cast<u32>(aTexture.width())};
    const u32 SizeY {static_cast<u32>(aTexture.height())};
    const f64 MinHeight {static_cast<f64>(aConfig.oMinHeight)};
    const f64 MaxHeight {static_cast<f64>(aConfig.oMaxHeight)};

    oHeights.resize(SizeX * SizeY);
    u32 VertexIndex {0U};

    // TODO: HK-42 Why dont we use more performing funcions, like constBits or ScanLine?
    for (u32 i {0U}; i < SizeX; i++)
    {
        for (u32 j {0U}; j < SizeY; j++)
        {
            f64 NormalizedHeight {qRed(aTexture.pixel(i, j)) / 255.0};
            oHeights[VertexIndex] = Math::Lerp(MinHeight, MaxHeight, NormalizedHeight);
            VertexIndex++;
        }
    }

    oResolution.oX = SizeX;
    oResolution.oY = SizeY;
}
