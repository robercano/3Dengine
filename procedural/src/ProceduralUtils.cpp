/**
 * @module  Procedural
 * @brief	Procedural objects generation library
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "ProceduralUtils.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Logging.hpp"
#include "Plane.hpp"

using namespace Logging;

#define PI 3.14159265358979323846

void Procedural::AppendBentPlane(Model3D &model, float width, float height, const glm::vec3 &color, float angleWidth, float angleHeight,
                                 float angleRadius, uint32_t numVertsWidth, uint32_t numVertsHeight)
{
    float radiusWidth, offsetWidth, angleIncrementWidth;
    float radiusHeight, offsetHeight, angleIncrementHeight;
    float angleIncrementRadius;

    glm::vec3 center(0.0f, 0.0f, 0.0f);

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
    model._modelData.resize(numVertsWidth * numVertsHeight);
    model._modelIndices.resize((size_t)(2 * 3 * numEdgesWidth * numEdgesHeight)); /** 2 triangles, 3 vertices each */

    Model3D::VertexData *data = &model._modelData[0];

    /* Set parameters for generation along the z-axis */
    if (angleWidth != 0.0f) {
        radiusWidth = width / angleWidth;
        offsetWidth = (float)(radiusWidth * glm::sin((PI + angleWidth) / 2.0f));
        center = glm::vec3(0.0f, -offsetWidth, 0.0f);
    } else {
        radiusWidth = halfWidth;
    }
    angleIncrementWidth = angleWidth / numEdgesWidth;

    /* Set parameters for generation along the x-axis */
    if (angleHeight != 0.0f) {
        radiusHeight = height / angleHeight;
        offsetHeight = radiusHeight * glm::sin((PI + angleHeight) / 2.0f);
        center += glm::vec3(0.0f, -offsetHeight, 0.0f);
    } else {
        radiusHeight = halfHeight;
    }
    angleIncrementHeight = angleHeight / numEdgesHeight;

    angleIncrementRadius = angleRadius / 2.0f / numEdgesHeight;

    float vertexAngleHeight = (float)((PI - angleHeight) / 2.0f);
    float vertexAngleRadius = (2.0f * PI - angleRadius) / 4.0f;

    for (unsigned int i = 0, count = 0; i < numVertsHeight;
         ++i, vertexAngleHeight += angleIncrementHeight, vertexAngleRadius += angleIncrementRadius) {
        glm::vec3 offset;
        glm::mat4 rotation(1.0f);

        if (angleWidth != 0.0f && angleHeight != 0.0f) {
            rotation = glm::toMat4(glm::quat(glm::vec3(vertexAngleHeight - PI / 2.0f, 0.0f, 0.0f)));
        }

        if (angleHeight != 0.0f) {
            offset = glm::vec3(0.0f, sin(vertexAngleHeight), -cos(vertexAngleHeight));
        } else if (angleRadius != 0.0f) {
            offset = glm::vec3(0.0f, 0.0f, -glm::cos(vertexAngleRadius));
        } else {
            offset = glm::vec3(0.0f, 0.0f, -1.0f + 2.0f * i / (float)numEdgesHeight);
        }

        float radius = radiusWidth * sin(vertexAngleRadius);

        float vertexAngleWidth = (float)((PI + angleWidth) / 2.0f);
        for (unsigned int j = 0; j < numVertsWidth; ++j, vertexAngleWidth -= angleIncrementWidth) {
            glm::vec3 unitVertex, normal;

            if (angleWidth != 0.0f) {
                unitVertex = glm::vec3(cos(vertexAngleWidth), sin(vertexAngleWidth), 0.0f);
                normal = unitVertex;
            } else {
                unitVertex = glm::vec3(-1.0 + 2 * j / (float)numEdgesWidth, 0.0f, 0.0f);
                normal = glm::vec3(0.0f, 1.0f, 0.0f);
            }

            unitVertex = glm::vec3(rotation * glm::vec4(unitVertex, 1.0f)) * radius + offset * radiusHeight;
            normal = glm::vec3(rotation * glm::vec4(normal, 1.0f));

            data[count].vertex = unitVertex + center;
            if (angleRadius != 0.0f) {
                data[count].normal = glm::normalize(unitVertex);
            } else {
                data[count].normal = normal;
            }
            count++;
        }
    }

    /* Generate the indices */
    uint32_t *index = &model._modelIndices[0];
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
    model._materials.push_back(Material());

    /* Add a default texture */
    uint8_t rgb[3] = {(uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255)};
    model._textures.push_back(Texture(rgb, 1, 1, 8));

    /* Only one set of indices */
    model._indicesOffsets.push_back(0);
    model._indicesCount.push_back(model._modelIndices.size());
}
