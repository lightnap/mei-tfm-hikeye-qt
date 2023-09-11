#include "CHeightMap.hpp"

#include "Types.hpp"

#include <QImage>

CHeightMap::CHeightMap(QImage& aTexture)
{
    mHeights.clear();

    // TODO: This could be moven to the height map resource loader so that it can be interrupted if it takes a long time.
    const u32 SizeX {static_cast<u32>(aTexture.width())};
    const u32 SizeY {static_cast<u32>(aTexture.height())};

    mHeights.resize(SizeX * SizeY);

    u32 VertexIndex {0U};

    // TODO: Why dont we use more performing funcions, like constBits or ScanLine?
    for (u32 i = 0U; i < SizeX; i++)
    {
        for (u32 j = 0U; j < SizeY; j++)
        {
            mHeights[VertexIndex] = qRed(aTexture.pixel(i, j)) / 255.0;
            VertexIndex++;
        }
    }
}
