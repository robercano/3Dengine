#include "Cylinder.hpp"
#include <glm/glm.hpp>
#include "Logging.hpp"
#include "ModelTransform.hpp"

using namespace Procedural;
using namespace Logging;

#define PI 3.14159265358979323846

Cylinder::Cylinder(uint32_t numSides, const glm::vec3 &color)
{
    glm::vec3 topCenter(0.0f, 1.0f, 0.0f);
    glm::vec3 bottomCenter(0.0f, -1.0f, 0.0f);

    /* 2 center vertices, then 2 vertices at top for up and side
     * direction, and then 2 at bottom for down and side direction */
    _modelData.resize(2 + 4 * numSides);

    /* numSides triangles at top, numSides triangles at bottom,
     * then 2 triangles for each side:
     *
     * total: numSides + numSides + 2*numSides
     */
    _modelIndices.resize(3 * 4 * numSides);

    /* Generate the 2 center vertices */
    _modelData[0].vertex = topCenter;
    _modelData[0].normal = topCenter;
    _modelData[1].vertex = bottomCenter;
    _modelData[1].normal = bottomCenter;

    /* Now generate the surrounding vertices for the top and
     * bottom lids */
    Model3D::VertexData *data = &_modelData[2];

    for (unsigned int i = 0, count = 0; i < numSides; ++i) {
        float angle = 2 * PI * i / numSides;

        glm::vec3 vertex = glm::vec3(glm::cos(angle), 0.0f, glm::sin(angle));
        glm::vec3 sideNormal = glm::normalize(vertex);

        /* Upper lid: same vertex twice with different normals to create the
         * top and the side lighting */
        data[count].vertex = vertex + topCenter;
        data[count].normal = topCenter; /* Top center is also in the direction of the normal we need */
        count++;

        data[count].vertex = vertex + topCenter;
        data[count].normal = sideNormal;
        count++;

        /* Bottom lid: same vertex twice with different normals to create the
         * top and the side lighting */
        data[count].vertex = vertex + bottomCenter;
        data[count].normal = bottomCenter; /* Bottom center is also in the direction of the normal we need */
        count++;

        data[count].vertex = vertex + bottomCenter;
        data[count].normal = sideNormal;
        count++;
    }

    /* Generate the indices now */
    uint32_t *index = &_modelIndices[0];
    uint32_t count = 0;

    /* Upper lid */
    for (int i = 0; i < numSides - 1; ++i) {
        index[count++] = 0;
        index[count++] = 2 + 4 * (i + 1);
        index[count++] = 2 + 4 * (i + 0);
    }
    /* Upper lid last triangle */
    index[count++] = 0;
    index[count++] = 2 + 4 * (0);
    index[count++] = 2 + 4 * (numSides - 1);

    /* Bottom lid */
    for (int i = 0; i < numSides - 1; ++i) {
        index[count++] = 1;
        index[count++] = 2 + 4 * (i + 0) + 2;
        index[count++] = 2 + 4 * (i + 1) + 2;
    }
    /* Bottom lid last triangle */
    index[count++] = 1;
    index[count++] = 2 + 4 * (numSides - 1) + 2;
    index[count++] = 2 + 4 * (0) + 2;

    /* Sides */
    for (int i = 0; i < numSides - 1; ++i) {
        index[count++] = 2 + 4 * (i + 0) + 1;
        index[count++] = 2 + 4 * (i + 1) + 3;
        index[count++] = 2 + 4 * (i + 0) + 3;

        index[count++] = 2 + 4 * (i + 0) + 1;
        index[count++] = 2 + 4 * (i + 1) + 1;
        index[count++] = 2 + 4 * (i + 1) + 3;
    }
    /* Last side */
    index[count++] = 2 + 4 * (numSides - 1) + 1;
    index[count++] = 2 + 4 * (0) + 3;
    index[count++] = 2 + 4 * (numSides - 1) + 3;

    index[count++] = 2 + 4 * (numSides - 1) + 1;
    index[count++] = 2 + 4 * (0) + 1;
    index[count++] = 2 + 4 * (0) + 3;

    _materials.push_back(Material());

    /* Add a default texture */
    uint8_t rgb[3] = {(uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255)};
    _textures.push_back(Texture(rgb, 1, 1, 8));

    /* Only one set of indices */
    _indicesOffsets.push_back(0);
    _indicesCount.push_back(_modelIndices.size());
}
