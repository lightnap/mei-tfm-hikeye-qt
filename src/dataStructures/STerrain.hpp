#ifndef C_TERRAIN_H
#define C_TERRAIN_H

#include "Math.hpp"
#include "Types.hpp"

#include <QOpenGLFunctions>
#include <vector>

struct SHeightMap;
struct STerrainConfig;

/**
 * @brief Class that represents a model of the terrain.
 */
struct STerrain
{
  public:
    /**
     * @brief Constructor.
     * @param aHeightMap: Reference to the height map we will use to construct the terrain.
     * @param aConfig: Extra information needed to construct the terrain.
     */
    STerrain(const SHeightMap& aHeightMap, const STerrainConfig& aConfig);

  private:
    /**
     * @brief Creates the vertices of the terrain model.
     * @param aHeightMap: HeightMap which is used for the creation.
     * @param aConfig: additional info needed to create the vertices.
     */
    void CreateVertices(const SHeightMap& aHeightMap, const STerrainConfig& aConfig);

    /**
     * @brief Creates the triangles of the terrain model.
     * @param aHeightMap: HeightMap which is used for the creation.
     * @param aConfig: additional info needed to create the triangles.
     */
    void CreateTriangles(const SHeightMap& aHeightMap);

    /**
     * @brief Creates the bounding box of the terrian model.
     * @param aHeightMap; Height map which is used for the creation.
     * @param aConfig: additional info needed to create the box.
     */
    void CreateBounds(const SHeightMap& aHeightMap, const STerrainConfig& aConfig);

  public:
    s32                  oTriangleCount; //!< Number of triangles in the model.
    std::vector<GLfloat> oVertices;      //!< Vector containing the vertices of the model. 3 entries make a vertex.
    std::vector<GLuint>  oTriangles;     //!< Vector containing vertex indices for triangles of the model.
    Math::Box3D          oBounds;        //!< Bounding box for the terrain.
};

#endif // C_TERRAIN_H
