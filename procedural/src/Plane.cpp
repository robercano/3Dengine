/**
 * @module  Procedural
 * @brief	Procedural objects generation library
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Plane.hpp"
#include <glm/glm.hpp>
#include "Logging.hpp"

using namespace Logging;
using namespace Procedural;

#define PI 3.14159265358979323846

Plane::Plane(float width, float height, const glm::vec3 &color, float angle, uint32_t numVertsWidth, uint32_t numVertsHeight)
    : _width(width), _height(height), _color(color), _angle(angle), _numVertsWidth(numVertsWidth), _numVertsHeight(numVertsHeight)
{
    if (_numVertsWidth < 2) {
        _numVertsWidth = 2;
    }
    if (_numVertsHeight < 2) {
        _numVertsHeight = 2;
    }

    float numEdgesWidth = (float)(_numVertsWidth - 1);
    float numEdgesHeight = (float)(_numVertsHeight - 1);

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    /*
     * For each row of vertices of the plane two indices are needed
     * per column, plus one at the beggining and other at the end
     * for degenerate triangles. That is number of 2*columns + 2.
     * Then multiplied by the number of rows minus one, as the last
     * row does have to generate more triangles
     */
    _modelData.resize(_numVertsWidth * _numVertsHeight);
    _modelIndices.resize((size_t)(2 * 3 * numEdgesWidth * numEdgesHeight)); /** 2 triangles, 3 vertices each */

    Model3D::VertexData *data = &_modelData[0];

    /* Generate the plane vertices */
    if (_angle != 0.0f) {
        /* Bent plane */
        float radius = _width / _angle;
        float offset = radius * glm::sin((PI + _angle) / 2.0f);
        float angleIncrement = _angle / numEdgesWidth;

        for (unsigned int i = 0, count = 0; i < _numVertsHeight; ++i) {
            float vertexAngle = (PI + _angle) / 2.0f;

            for (unsigned int j = 0; j < _numVertsWidth; ++j, vertexAngle -= angleIncrement) {
                data[count].vertex = glm::vec3(radius * cos(vertexAngle), radius * sin(vertexAngle) - offset,
                                               -halfHeight + height * i / (float)numEdgesHeight);
                data[count].normal = glm::vec3(0.0f, 1.0f, 0.0f);
                data[count].uvcoord = glm::vec2(0.0f, 0.0f);
                count++;
            }
        }
    } else {
        /* Straight plane */
        for (unsigned int i = 0, count = 0; i < _numVertsHeight; ++i) {
            for (unsigned int j = 0; j < _numVertsWidth; ++j) {
                data[count].vertex =
                    glm::vec3(-halfWidth + width * j / (float)numEdgesWidth, 0.0f, -halfHeight + height * i / (float)numEdgesHeight);
                data[count].normal = glm::vec3(0.0f, 1.0f, 0.0f);
                data[count].uvcoord = glm::vec2(0.0f, 0.0f);

                count++;
            }
        }
    }

    /* Generate the indices */
    uint32_t *index = &_modelIndices[0];
    for (unsigned int i = 0, count = 0; i < numEdgesHeight; ++i) {
        for (unsigned int j = 0; j < numEdgesWidth; ++j) {
            uint32_t span = i * _numVertsWidth;
            index[count++] = j + span;
            index[count++] = j + span + _numVertsWidth;
            index[count++] = j + span + 1;

            index[count++] = j + span + 1;
            index[count++] = j + span + _numVertsWidth;
            index[count++] = j + span + _numVertsWidth + 1;
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
