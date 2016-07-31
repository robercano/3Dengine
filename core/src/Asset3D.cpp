/**
 * @class	BlinnPhongShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "Asset3D.hpp"
#include "OpenGLAsset3D.hpp"

Asset3D *Asset3D::New(void) { return new OpenGLAsset3D(); }
void Asset3D::Delete(Asset3D *asset) { delete asset; }
void Asset3D::normalize()
{
    std::vector<VertexData>::iterator it;
    glm::vec3 cm = glm::vec3(0.0f, 0.0f, 0.0f);

    for (it = _vertexData.begin(); it != _vertexData.end(); ++it) {
        cm += it->vertex;
    }
    cm /= _vertexData.size();

    /* Substract the center of mass to all vertices */
    float maxLength = std::numeric_limits<float>::min();

    for (it = _vertexData.begin(); it != _vertexData.end(); ++it) {
        it->vertex -= cm;

        /* Calculate maximum length */
        float length = glm::length(it->vertex);
        if (length > maxLength) {
            maxLength = length;
        }
    }

    /* Finally divide by maxLength to make the model fit in a sphere
     * of radius 1.0 */
    for (it = _vertexData.begin(); it != _vertexData.end(); ++it) {
        it->vertex /= maxLength;
    }
}
