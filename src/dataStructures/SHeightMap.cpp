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

    // TODO: HK-42 Why dont we use more performing funcions, like constBits or constScanLine?
    for (u32 Column {0U}; Column < SizeX; Column++)
    {
        for (u32 Row {0U}; Row < SizeY; Row++)
        {
            f64 NormalizedHeight {aTexture.pixelColor(Column, Row).blueF()};
            oHeights[VertexIndex] = Math::Lerp(MinHeight, MaxHeight, NormalizedHeight);
            VertexIndex++;
        }
    }

    oResolution.oX = SizeX;
    oResolution.oY = SizeY;
}
