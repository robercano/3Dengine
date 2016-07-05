/**
 * @module  Procedural
 * @brief	Procedural objects generation library
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Plane.hpp"
#include <glm/glm.hpp>

using namespace Procedural;

Plane::Plane(uint32_t numVertices, const glm::vec3 &color)
{
    if (numVertices < 2) {
        numVertices = 2;
    }

    float numFaces = (float)(numVertices - 1);

    /*
     * For each row of vertices of the plane two indices are needed
     * per column, plus one at the beggining and other at the end
     * for degenerate triangles. That is number of 2*columns + 2.
     * Then multiplied by the number of rows minus one, as the last
     * row does have to generate more triangles
     */
    _modelData.resize(numVertices * numVertices);
    _modelIndices.resize((size_t)(2 * 3 * numFaces * numFaces)); /** 2 triangles, 3 vertices each */

    Model3D::VertexData *data = &_modelData[0];

    /* Generate the plane vertices */
    for (unsigned int i = 0, count = 0; i < numVertices; ++i) {
        for (unsigned int j = 0; j < numVertices; ++j) {
            data[count].vertex = glm::vec3(-0.5f + j / (float)numFaces, 0.0f, -0.5 + i / (float)numFaces);
            data[count].normal = glm::vec3(0.0f, 1.0f, 0.0f);
            data[count].uvcoord = glm::vec2(j / (float)numFaces, 1.0f - i / (float)numFaces);

            count++;
        }
    }

    /* Generate the indices */
    uint32_t *index = &_modelIndices[0];
    for (unsigned int i = 0, count = 0; i < numFaces; ++i) {
        for (unsigned int j = 0; j < numFaces; ++j) {
            uint32_t span = i * numVertices;
            index[count++] = j + span;
            index[count++] = j + span + numVertices;
            index[count++] = j + span + 1;

            index[count++] = j + span + 1;
            index[count++] = j + span + numVertices;
            index[count++] = j + span + numVertices + 1;
        }
    }

    /* Add a default material */
    _materials.push_back(Material());

    /* Add a default texture */
    uint8_t rgb[3] = {(uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255)};
    _textures.push_back(Texture(rgb, 1, 1, 8));

    /* Only one set of indices */
    _indicesOffsets.push_back(0);
    _indicesCount.push_back(_modelIndices.size());
}
