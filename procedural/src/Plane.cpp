/**
 * @module  Procedural
 * @brief	Procedural objects generation library
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Plane.hpp"
#include <glm/glm.hpp>

using namespace Procedural;

Plane::Plane(float width, float height, const glm::vec3 &color, uint32_t numVertsWidth, uint32_t numVertsHeight)
{
    if (numVertsWidth < 2) {
        numVertsWidth = 2;
    }
    if (numVertsHeight < 2) {
        numVertsHeight = 2;
    }

    float numEdgesWidth = (float)(numVertsWidth - 1);
    float numEdgesHeight = (float)(numVertsHeight - 1);

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    /*
     * For each row of vertices of the plane two indices are needed
     * per column, plus one at the beggining and other at the end
     * for degenerate triangles. That is number of 2*columns + 2.
     * Then multiplied by the number of rows minus one, as the last
     * row does have to generate more triangles
     */
    _modelData.resize(numVertsWidth * numVertsHeight);
    _modelIndices.resize((size_t)(2 * 3 * numEdgesWidth * numEdgesHeight)); /** 2 triangles, 3 vertices each */

    Model3D::VertexData *data = &_modelData[0];

    /* Generate the plane vertices */
    for (unsigned int i = 0, count = 0; i < numVertsHeight; ++i) {
        for (unsigned int j = 0; j < numVertsWidth; ++j) {
            data[count].vertex =
                glm::vec3(-halfWidth + width * j / (float)numEdgesWidth, 0.0f, -halfHeight + height * i / (float)numEdgesHeight);
            data[count].normal = glm::vec3(0.0f, 1.0f, 0.0f);
            data[count].uvcoord = glm::vec2(j / (float)numEdgesWidth, 1.0f - i / (float)numEdgesHeight);

            count++;
        }
    }

    /* Generate the indices */
    uint32_t *index = &_modelIndices[0];
    for (unsigned int i = 0, count = 0; i < numEdgesHeight; ++i) {
        for (unsigned int j = 0; j < numEdgesWidth; ++j) {
            uint32_t span = i * numVertsWidth;
            index[count++] = j + span;
            index[count++] = j + span + numVertsWidth;
            index[count++] = j + span + 1;

            index[count++] = j + span + 1;
            index[count++] = j + span + numVertsWidth;
            index[count++] = j + span + numVertsWidth + 1;
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
