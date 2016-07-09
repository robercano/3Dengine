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
#include "ModelTransform.hpp"

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
    bool rotatePlane = false;

    if (_numVertsWidth < 2) {
        _numVertsWidth = 2;
    }
    if (_numVertsHeight < 2) {
        _numVertsHeight = 2;
    }
    /* Only consider 3 cases, no bending angle, two bending angles,
     * or only one bending angle (either _angleWidth or _angleHeight. For this
     * last case if only _angleHeight is defined we move to _angleWidth and set
     * _angleHeight to 0.0f. After the process we rotate the plane to put the
     * bending angle along the z-axis instead of along the x-axis */
    if (_angleWidth == 0.0f && _angleHeight != 0.0f) {
        _angleWidth = _angleHeight;
        _angleHeight = 0.0f;
        rotatePlane = true;
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

    /* Straight plane */
    if (_angleWidth == 0.0f && _angleHeight == 0.0f) {
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
    }
    /* Bent plane along the x-axis */
    if (_angleWidth != 0.0f) {
        float radiusWidth, offsetWidth, angleIncrementWidth;
        float radiusHeight, offsetHeight, angleIncrementHeight;

        radiusWidth = _width / _angleWidth;
        offsetWidth = radiusWidth * glm::sin((PI + _angleWidth) / 2.0f);
        angleIncrementWidth = _angleWidth / numEdgesWidth;

        if (_angleHeight != 0.0f) {
            radiusHeight = _height / _angleHeight;
            offsetHeight = radiusHeight * glm::sin((PI + _angleHeight) / 2.0f);
            angleIncrementHeight = _angleHeight / numEdgesHeight;
        } else {
            radiusHeight = halfHeight;
            angleIncrementHeight = _angleHeight / numEdgesHeight;
        }

        float vertexAngleHeight = (PI - _angleHeight) / 2.0f;
        for (unsigned int i = 0, count = 0; i < _numVertsHeight; ++i, vertexAngleHeight += angleIncrementHeight) {
            float vertexAngleWidth = (PI + _angleWidth) / 2.0f;

            for (unsigned int j = 0; j < _numVertsWidth; ++j, vertexAngleWidth -= angleIncrementWidth) {
                glm::vec3 unitVertex(cos(vertexAngleWidth), sin(vertexAngleWidth), 0.0f);
                glm::vec3 offset, center;
                glm::mat4 rotation;

                if (_angleHeight != 0.0f) {
                    offset = glm::vec3(0.0f, sin(vertexAngleHeight), -cos(vertexAngleHeight));
                    center = glm::vec3(0.0f, -offsetWidth - offsetHeight, 0.0f);
                    rotation = glm::toMat4(glm::quat(glm::vec3(vertexAngleHeight - PI / 2.0f, 0.0f, 0.0f)));
                    unitVertex = glm::vec3(rotation * glm::vec4(unitVertex, 1.0f));
                } else {
                    offset = glm::vec3(0.0f, 0.0f, -1.0f + 2.0f * i / (float)numEdgesHeight);
                    center = glm::vec3(0.0f, -offsetWidth, 0.0f);
                }

                data[count].vertex = unitVertex * radiusWidth + offset * radiusHeight + center;
                data[count].normal = unitVertex;
                count++;
            }
        }
    }
    if (rotatePlane) {
        ModelTransform::Rotate(*this, glm::vec3(0.0f, PI / 2.0f, 0.0f));
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
