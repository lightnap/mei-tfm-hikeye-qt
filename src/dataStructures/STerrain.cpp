#include "STerrain.hpp"

#include "CConfigs.hpp"
#include "SHeightMap.hpp"
#include "Types.hpp"

#include <QOpenGLFunctions>

#include <vector>

STerrain::STerrain(const SHeightMap& aHeightMap, const STerrainConfig& aConfig)
{
    CreateVertices(aHeightMap, aConfig);
    CreateTriangles(aHeightMap);
}

void STerrain::CreateVertices(const SHeightMap& aHeightMap, const STerrainConfig& aConfig)
{
    s32 RowCount = aHeightMap.oResolution.oX;
    s32 ColumnCount = aHeightMap.oResolution.oY;

    Types::SVector2D Origin {aConfig.oBounds.oMin};
    f64              CellSize {aConfig.oCellSize};

    oVertices.clear();
    oVertices.reserve(3 * RowCount * ColumnCount);

    u32 Index {0U};
    for (s32 Row {0}; Row < RowCount; Row++)
    {
        for (s32 Column {0}; Column < ColumnCount; Column++)
        {
            oVertices[Index++] = GLfloat(Origin.oX + Row * CellSize);
            oVertices[Index++] = GLfloat(Origin.oY + Column * CellSize);
            oVertices[Index++] = GLfloat(aHeightMap.oHeights[Row * ColumnCount + Column]);
        }
    }
}

void STerrain::CreateTriangles(const SHeightMap& aHeightMap)
{
    s32  RowCount = aHeightMap.oResolution.oX;
    s32  ColumnCount = aHeightMap.oResolution.oY;
    s32& RowSize {ColumnCount};
    s32& ColumnSize {RowCount};

    s32 TrianglesCount = 2 * (RowSize - 1) * (ColumnSize - 1);

    oTriangles.clear();
    oTriangles.reserve(3 * TrianglesCount);

    u32 Index {0U};
    for (s32 Row {1}; Row < RowCount; Row++)
    {
        for (s32 Column {1}; Column < ColumnCount; Column++)
        {
            GLuint v00 = (Row - 1) * RowSize + Column - 1;
            GLuint v01 = (Row - 1) * RowSize + Column;
            GLuint v10 = Row * RowSize + Column - 1;
            GLuint v11 = Row * RowSize + Column;

            oTriangles[Index++] = v00;
            oTriangles[Index++] = v01;
            oTriangles[Index++] = v10;

            oTriangles[Index++] = v10;
            oTriangles[Index++] = v01;
            oTriangles[Index++] = v11;
        }
    }
}
