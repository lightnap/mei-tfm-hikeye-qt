#include "SHeightMap.hpp"

#include "CConfigs.hpp"
#include "Math.hpp"
#include "Types.hpp"

#include <QImage>

SHeightMap::SHeightMap(QImage& aTexture, const SHeightMapConfig& aConfig)
  : oMaxHeight(aConfig.oMaxHeight)
  , oMinHeight(aConfig.oMinHeight)
{
    oHeights.clear();

    const u32 SizeX {static_cast<u32>(aTexture.width())};
    const u32 SizeY {static_cast<u32>(aTexture.height())};
    const f64 MinHeight {static_cast<f64>(aConfig.oMinHeight)};
    const f64 MaxHeight {static_cast<f64>(aConfig.oMaxHeight)};

    oHeights.resize(SizeX * SizeY);
    u32 VertexIndex {0U};

    // TODO: Why dont we use more performing funcions, like constBits or ScanLine?
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
