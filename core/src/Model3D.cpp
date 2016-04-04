/**
 * @class	Model3D
 * @brief	Basic model 3D representation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <limits>
#include "Model3D.hpp"

void Model3D::_calculateCenterOfMass(glm::vec3 &cm)
{
    std::vector<Model3D::VertexData>::iterator it;
    cm = glm::vec3(0.0f, 0.0f, 0.0f);

    for (it = _modelData.begin(); it != _modelData.end(); ++it) {
        cm += it->vertex;
    }
    cm /= _modelData.size();
}

void Model3D::normalize()
{
    std::vector<Model3D::VertexData>::iterator it;
    glm::vec3 cm;

    _calculateCenterOfMass(cm);

    /* Substract the center of mass to all vertices */
    float min_Y = std::numeric_limits<float>::infinity();
    float max_length = std::numeric_limits<float>::min();

    for (it = _modelData.begin(); it != _modelData.end(); ++it) {
        it->vertex -= cm;

        /* Calculate maximum length */
        float length = glm::length(it->vertex);
        if (length > max_length) {
            max_length = length;
        }

        /* Calculate minimum Y coordinate */
        if (it->vertex.y < min_Y) {
            min_Y = it->vertex.y;
        }
    }

    /* Finally bottom of model to the XZ plane by substracting min_Y
     * and then divide by max_length to make the model fit in a sphere
     * of radius 1.0 */
    for (it = _modelData.begin(); it != _modelData.end(); ++it) {
        it->vertex.y -= min_Y;
        it->vertex /= max_length;
    }
}
