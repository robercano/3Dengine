/**
 * @class	OpenGLShaderPointLight
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string.h>
#include "OpenGLShaderPointLight.hpp"
#include <glm/glm.hpp>

void OpenGLShaderPointLight::init(uint32_t bindingPoint, uint32_t lightIndex)
{
    setBlockName("PointLight");
    setBindingPoint(bindingPoint);
    setBlockArrayIndex(lightIndex);
    addParamName("position");
    addParamName("ambient");
    addParamName("diffuse");
    addParamName("specular");
    addParamName("attenuation");
    addParamName("cutoff");
}

void OpenGLShaderPointLight::copyLight(PointLight &light)
{
    setParamValue("position",    light.getPosition());
    setParamValue("ambient",     light.getAmbient());
    setParamValue("diffuse",     light.getDiffuse());
    setParamValue("specular",    light.getSpecular());
    setParamValue("attenuation", light.getAttenuation());
    setParamValue("cutoff",      light.getCutoff());
    bind();
}
