/**
 * @class	OpenGLShaderLight
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string.h>
#include "OpenGLShaderLight.hpp"
#include <glm/glm.hpp>

OpenGLShaderLight::OpenGLShaderLight(uint32_t bindingPoint, uint32_t lightIndex)
{
    setBlockName("Light");
    setBindingPoint(bindingPoint);
    setBlockArrayIndex(lightIndex);
    addParamName("position");
    addParamName("ambient");
    addParamName("diffuse");
    addParamName("specular");
}

bool OpenGLShaderLight::copyLight(Light &light)
{
    setParamValue("position", light.getPosition());
    setParamValue("ambient",  light.getAmbient());
    setParamValue("diffuse",  light.getDiffuse());
    setParamValue("specular", light.getSpecular());
    bind();
    return true;
}
