/**
 * @module  Procedural
 * @brief	Procedural objects generation library
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Plane.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Logging.hpp"

using namespace Logging;
using namespace Procedural;

#define PI 3.14159265358979323846

Plane::Plane(float width, float height, const glm::vec3 &color, float angleWidth, float angleHeight, uint32_t numVertsWidth,
             uint32_t numVertsHeight)
    : _width(width)
    , _height(height)
    , _color(color)
    , _angleWidth(angleWidth)
    , _angleHeight(angleHeight)
    , _numVertsWidth(numVertsWidth)
    , _numVertsHeight(numVertsHeight)
{
    if (_numVertsWidth < 2) {
        _numVertsWidth = 2;
    }
    if (_numVertsHeight < 2) {
        _numVertsHeight = 2;
    }

    float numEdgesWidth = (float)(_numVertsWidth - 1);
    float numEdgesHeight = (float)(_numVertsHeight - 1);

    float halfWidth = _width / 2.0f;
    float halfHeight = _height / 2.0f;

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

    /* Generate the straight plane first */
    for (unsigned int i = 0, count = 0; i < _numVertsHeight; ++i) {
        for (unsigned int j = 0; j < _numVertsWidth; ++j) {
            data[count].vertex.x = -halfWidth + width * j / (float)numEdgesWidth;
            data[count].vertex.y = 0.0f;
            data[count].vertex.z = -halfHeight + height * i / (float)numEdgesHeight;

            data[count].normal = glm::vec3(0.0f, 1.0f, 0.0f);
            data[count].uvcoord = glm::vec2(0.0f, 0.0f);

            count++;
        }
    }
    /* Now check if we need to generate different coordinates for a bent plane */
    if (_angleWidth != 0.0f) {
        /* Bent plane around the z-axis */
        float radiusWidth = _width / _angleWidth;
        float offsetWidth = radiusWidth * glm::sin((PI + _angleWidth) / 2.0f);
        float angleIncrementWidth = _angleWidth / numEdgesWidth;

        for (unsigned int i = 0, count = 0; i < _numVertsHeight; ++i) {
            float vertexAngleWidth = (PI + _angleWidth) / 2.0f;

            for (unsigned int j = 0; j < _numVertsWidth; ++j, vertexAngleWidth -= angleIncrementWidth) {
                glm::vec3 unitVertex(cos(vertexAngleWidth), sin(vertexAngleWidth), 0.0f);

                data[count].vertex =
                    unitVertex * radiusWidth + glm::vec3(0.0f, -offsetWidth, -halfHeight + _height * i / (float)numEdgesHeight);
                /* Don't touch the z coordinate here */
                data[count].normal = unitVertex;
                count++;
            }
        }
    }
    if (_angleWidth == 0.0f && _angleHeight != 0.0f) {
        /* Bent plane only around the x-axis */
        float radiusHeight = _height / _angleHeight;
        float offsetHeight = radiusHeight * glm::sin((PI + _angleHeight) / 2.0f);
        float angleIncrementHeight = _angleHeight / numEdgesHeight;

        float vertexAngleHeight = (PI + _angleHeight) / 2.0f;
        for (unsigned int i = 0, count = 0; i < _numVertsHeight; ++i, vertexAngleHeight -= angleIncrementHeight) {
            for (unsigned int j = 0; j < _numVertsWidth; ++j) {
                glm::vec3 unitVertex(0.0f, sin(vertexAngleHeight), cos(vertexAngleHeight));

                /* Don't touch the x coordinate */
                data[count].vertex =
                    unitVertex * radiusHeight + glm::vec3(-halfWidth + _width * j / (float)numEdgesWidth, -offsetHeight, 0.0f);
                data[count].normal = unitVertex;
                count++;
            }
        }
    }
    if (_angleWidth != 0.0f && _angleHeight != 0.0f) {
        /* TODO */
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
