#include "STerrain.hpp"

#include "common/Math.hpp"
#include "common/Types.hpp"
#include "dataStructures/CConfigs.hpp"
#include "dataStructures/SHeightMap.hpp"

#include <QOpenGLFunctions>

#include <iostream> // TODO: Delete this.
#include <vector>

STerrain::STerrain(const SHeightMap& aHeightMap, const STerrainConfig& aConfig)
{
    oTriangleCount = 0;
    CreateBounds(aHeightMap, aConfig);
    CreateVertices(aHeightMap, aConfig);
    CreateTriangles(aHeightMap);
    CreateNormals(aHeightMap, aConfig);
}

void STerrain::CreateBounds(const SHeightMap& aHeightMap, const STerrainConfig& aConfig)
{
    oBounds.oMin = Math::Vector3D(aConfig.oBounds.oMin.oX, aConfig.oBounds.oMin.oY, aHeightMap.oMinHeight);
    oBounds.oMax = Math::Vector3D(aConfig.oBounds.oMax.oX, aConfig.oBounds.oMax.oY, aHeightMap.oMaxHeight);
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
    // TODO: This seems to be the other way around??
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

void STerrain::CreateNormals(const SHeightMap& aHeightMap, const STerrainConfig& aConfig)
{
    s32 RowCount = aHeightMap.oResolution.oY;
    s32 ColumnCount = aHeightMap.oResolution.oX;

    std::cout << "ColumnCount: " << ColumnCount << " RowCount: " << RowCount << std::endl;

    oNormals.clear();
    oNormals.reserve(3 * RowCount * ColumnCount);

    for (s32 Row {0}; Row < RowCount; Row++)
    {
        for (s32 Column {0}; Column < ColumnCount; Column++)
        {
            Math::Vector3D Normal {CreateNormal(Row, Column)};

            oNormals.push_back(Normal.oX);
            oNormals.push_back(Normal.oY);
            oNormals.push_back(Normal.oZ);
        }
    }
}

Math::Vector3D STerrain::CreateNormal(s32 Row, s32 Column)
{
    // TODO: Do not hardcode these.
    s32 RowCount {4017};
    s32 ColumnCount {3725};

    Math::Vector3D Normal {0.0, 0.0, 1.0};
    /*if (Column == 0)
    {
        std::cout << "Computing normal Row: " << Row << " Column: " << Column << " Index: " << Column * 4017 + Row << std::endl; // TODO: Delete this
    }*/

    if (Row == 0)
    {
        if (Column == 0)
        {
            // Corner: 0/1
            Normal = Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row + 1, Column), GetVertex(Row + 1, Column + 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row + 1, Column + 1), GetVertex(Row, Column + 1));
        }
        else if (Column == ColumnCount - 1)
        {
            // Corner: 5
            Normal = Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row, Column - 1), GetVertex(Row + 1, Column));
        }
        else
        {
            // Edge: 0/1/5
            Normal = Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row + 1, Column), GetVertex(Row + 1, Column + 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row + 1, Column + 1), GetVertex(Row, Column + 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row, Column - 1), GetVertex(Row + 1, Column));
        }
    }
    else if (Row == RowCount - 1)
    {
        if (Column == 0)
        {
            // Corner: 2
            Normal = Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row, Column + 1), GetVertex(Row - 1, Column));
        }
        else if (Column == ColumnCount - 1)
        {
            // Corner: 3/4
            Normal = Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row - 1, Column - 1), GetVertex(Row, Column - 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row - 1, Column), GetVertex(Row - 1, Column - 1));
        }
        else
        {
            // Edge: 2/3/4
            Normal = Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row, Column + 1), GetVertex(Row - 1, Column)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row - 1, Column), GetVertex(Row - 1, Column - 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row - 1, Column - 1), GetVertex(Row, Column - 1));
        }
    }
    else
    {
        if (Column == 0)
        {
            // Edge: 0/1/2
            Normal = Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row + 1, Column), GetVertex(Row + 1, Column + 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row + 1, Column + 1), GetVertex(Row, Column + 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row, Column + 1), GetVertex(Row - 1, Column));
        }
        else if (Column == ColumnCount - 1)
        {
            // Edge: 3/4/5
            Normal = Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row - 1, Column), GetVertex(Row - 1, Column - 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row - 1, Column - 1), GetVertex(Row, Column - 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row, Column - 1), GetVertex(Row + 1, Column));
        }
        else
        {
            // Face: 0/1/2/3/4/5
            Normal = Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row + 1, Column), GetVertex(Row + 1, Column + 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row + 1, Column + 1), GetVertex(Row, Column + 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row, Column + 1), GetVertex(Row - 1, Column)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row - 1, Column), GetVertex(Row - 1, Column - 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row - 1, Column - 1), GetVertex(Row, Column - 1)) +
                     Math::GetTrianglePerpendicular(GetVertex(Row, Column), GetVertex(Row, Column - 1), GetVertex(Row + 1, Column));
        }
    }

    return Math::Normalize(Normal);
}

Math::Vector3D STerrain::GetVertex(s32 aRow, s32 aColumn) const
{
    // TODO: This should not be hardcoded.
    Math::Vector3D Vertex {0.0};
    // const s32      VertexIndex {3 * (aRow * 3725 + aColumn)};
    const s32 VertexIndex {3 * (aColumn * 4017 + aRow)};

    Vertex.oX = oVertices.at(VertexIndex);
    Vertex.oY = oVertices.at(VertexIndex + 1);
    Vertex.oZ = oVertices.at(VertexIndex + 2);

    return Vertex;
}

Math::Vector3D STerrain::GetNormal(s32 aRow, s32 aColumn) const
{
    // TODO: This should not be hardcoded.
    // TODO: GetVertex index function to share functionality with GetVertex().
    Math::Vector3D Normal {0.0};
    const s32      VertexIndex {3 * (aRow * 3725 + aColumn)};
    // const s32 VertexIndex {3 * (aColumn * 4017 + aRow)};

    Normal.oX = oNormals.at(VertexIndex);
    Normal.oY = oNormals.at(VertexIndex + 1);
    Normal.oZ = oNormals.at(VertexIndex + 2);

    return Normal;
}
