/**
 * @class	OpenGLMaterial
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string.h>
#include "OpenGLMaterial.hpp"
#include <glm/glm.hpp>

OpenGLMaterial::OpenGLMaterial()
{
    setBlockName("Material");
    addParamName("ambient");
    addParamName("diffuse");
    addParamName("specular");
    addParamName("alpha");
    addParamName("shininess");
}

bool OpenGLMaterial::setValues(glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular, float alpha, float shininess)
{
    setParamValue("ambient",   ambient);
    setParamValue("diffuse",   diffuse);
    setParamValue("specular",  specular);
    setParamValue("alpha",     alpha);
    setParamValue("shininess", shininess);
    bind();
    return true;
}
