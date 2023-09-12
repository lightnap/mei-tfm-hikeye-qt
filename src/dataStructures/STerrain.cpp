#include "STerrain.hpp"

#include "Types.hpp"
#include <vector>

STerrain::STerrain(const std::vector<f64>& aHeights)
{
    // TODO: Maybe move this to the resource loader so that it can be interrupted by user.

    // TODO: FIll this function:
    // Create vector of vertices
    // Create vector of triangles
    // Create a structure containign both

    // verts
    /*int                  idx = 0;
    std::vector<GLfloat> verts(3 * nx * ny);

    for (int i = 0; i < nx; i++)
    {
        for (int j = 0; j < ny; j++)
        {
            Vector3 p = hf.vertex(i, j);
            verts[idx++] = GLfloat(p[0]);
            verts[idx++] = GLfloat(p[1]);
            verts[idx++] = GLfloat(p[2]);
        }
    }

    // tris
    numTriangles = (nx - 1) * (ny - 1) * 2;
    idx = 0;
    std::vector<GLuint> indices(numTriangles * 3);
    for (int i = 1; i < nx; i++)
    {
        for (int j = 1; j < ny; j++)
        {
            GLuint v00 = (i - 1) * ny + j - 1;
            GLuint v01 = (i - 1) * ny + j;
            GLuint v10 = i * ny + j - 1;
            GLuint v11 = i * ny + j;

            indices[idx++] = v00;
            indices[idx++] = v01;
            indices[idx++] = v10;

            indices[idx++] = v10;
            indices[idx++] = v01;
            indices[idx++] = v11;
        }
    }*/
}
