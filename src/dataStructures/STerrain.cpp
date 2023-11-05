#include "STerrain.hpp"

#include "CConfigs.hpp"
#include "Math.hpp"

#include "SHeightMap.hpp"
#include "Types.hpp"

#include <QOpenGLFunctions>

#include <vector>

STerrain::STerrain(const SHeightMap& aHeightMap, const STerrainConfig& aConfig)
{
    oTriangleCount = 0;
    CreateVertices(aHeightMap, aConfig);
    CreateTriangles(aHeightMap);
    CreateBounds(aHeightMap, aConfig);
}

void STerrain::CreateVertices(const SHeightMap& aHeightMap, const STerrainConfig& aConfig)
{
    s32 RowCount = aHeightMap.oResolution.oX;
    s32 ColumnCount = aHeightMap.oResolution.oY;

    Math::Vector2D<f64> Origin {aConfig.oBounds.oMin};
    f64                 CellSize {aConfig.oCellSize};

    oVertices.clear();
    oVertices.reserve(3 * RowCount * ColumnCount);

    for (s32 Row {0}; Row < RowCount; Row++)
    {
        for (s32 Column {0}; Column < ColumnCount; Column++)
        {
            oVertices.push_back(GLfloat(Origin.oX + Row * CellSize));
            oVertices.push_back(GLfloat(Origin.oY + Column * CellSize));
            oVertices.push_back(GLfloat(aHeightMap.oHeights[Row * ColumnCount + Column]));
        }
    }
}

void STerrain::CreateTriangles(const SHeightMap& aHeightMap)
{
    s32  RowCount = aHeightMap.oResolution.oX;
    s32  ColumnCount = aHeightMap.oResolution.oY;
    s32& RowSize {ColumnCount};
    s32& ColumnSize {RowCount};

    oTriangleCount = 2 * (RowSize - 1) * (ColumnSize - 1);
    oTriangles.clear();
    oTriangles.reserve(3 * oTriangleCount);

    for (s32 Row {1}; Row < RowCount; Row++)
    {
        for (s32 Column {1}; Column < ColumnCount; Column++)
        {
            GLuint v00 = (Row - 1) * RowSize + Column - 1;
            GLuint v01 = (Row - 1) * RowSize + Column;
            GLuint v10 = Row * RowSize + Column - 1;
            GLuint v11 = Row * RowSize + Column;

            oTriangles.push_back(v00);
            oTriangles.push_back(v01);
            oTriangles.push_back(v10);

            oTriangles.push_back(v10);
            oTriangles.push_back(v01);
            oTriangles.push_back(v11);
        }
    }
}

void STerrain::CreateBounds(const SHeightMap& aHeightMap, const STerrainConfig& aConfig)
{
    oBounds.oMin = Math::Vector3D(aConfig.oBounds.oMin.oX, aConfig.oBounds.oMin.oY, aHeightMap.oMinHeight);
    oBounds.oMax = Math::Vector3D(aConfig.oBounds.oMax.oX, aConfig.oBounds.oMax.oY, aHeightMap.oMaxHeight);
}
