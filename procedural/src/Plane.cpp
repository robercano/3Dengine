/**
 * @module  Procedural
 * @brief	Procedural objects generation library
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Plane.hpp"
#include <glm/glm.hpp>
#include "Logging.hpp"

using namespace Procedural;
using namespace std;

Plane::Plane(uint32_t horizontal, uint32_t vertical)
{
    if (horizontal < 2) {
        horizontal = 2;
    }
    if (vertical < 2) {
        vertical = 2;
    }

    float hDiv = (float)(horizontal - 1);
    float vDiv = (float)(vertical - 1);

    /*
     * For each row of vertices of the plane two indices are needed
     * per column, plus one at the beggining and other at the end
     * for degenerate triangles. That is number of 2*columns + 2.
     * Then multiplied by the number of rows minus one, as the last
     * row does have to generate more triangles
     */
    _modelData.resize(horizontal * vertical);
    _modelIndices.resize((size_t)(2 * 3 * hDiv * vDiv)); /** 2 triangles, 3 vertices each */

    Model3D::VertexData *data = &_modelData[0];

    /* Generate the plane vertices */
    for (unsigned int i = 0, count = 0; i < vertical; ++i) {
        for (unsigned int j = 0; j < horizontal; ++j) {
            data[count].vertex = glm::vec3(-0.5f + j/(float)hDiv, 0.0f, -0.5 + i/(float)vDiv);
            data[count].normal = glm::vec3(0.0f, 1.0f, 0.0f);
            data[count].uvcoord = glm::vec2(j/(float)hDiv, 1.0f - i/(float)vDiv);

            count++;
        }
    }

    /* Generate the indices */
    uint32_t *index = &_modelIndices[0];
    for (unsigned int i = 0, count = 0; i < vertical - 1; ++i) {
        for (unsigned int j = 0; j < horizontal - 1; ++j) {
            uint32_t span = i * horizontal;
            index[count++] = j + span;
            index[count++] = j + span + horizontal;
            index[count++] = j + span + 1;

            index[count++] = j + span + 1;
            index[count++] = j + span + horizontal;
            index[count++] = j + span + horizontal + 1;
        }
    }

    /* Add a default material */
    _materials.push_back(Material());

    /* Add a default texture */
    uint8_t rgb[3] = {255, 255, 255};
    _textures.push_back(Texture(rgb, 1, 1, 8));

    /* Only one set of indices */
    _indicesOffsets.push_back(0);
    _indicesCount.push_back(_modelIndices.size());
}
