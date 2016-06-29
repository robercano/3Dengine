/**
 * @class	OpenGLShaderSpotLight
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "OpenGLShaderSpotLight.hpp"
#include <string.h>
#include <glm/glm.hpp>

void OpenGLShaderSpotLight::init(uint32_t bindingPoint, uint32_t lightIndex)
{
    setBlockName("SpotLight");
    setBindingPoint(bindingPoint);
    setBlockArrayIndex(lightIndex);
    addParamName("position");
    addParamName("direction");
    addParamName("ambient");
    addParamName("diffuse");
    addParamName("specular");
    addParamName("coneAngle");
    addParamName("conePenumbra");
    addParamName("attenuation");
    addParamName("cutoff");
}

void OpenGLShaderSpotLight::copyLight(SpotLight &light)
{
    setParamValue("position", light.getPosition());
    setParamValue("direction", light.getDirection());
    setParamValue("ambient", light.getAmbient());
    setParamValue("diffuse", light.getDiffuse());
    setParamValue("specular", light.getSpecular());
    setParamValue("coneAngle", light.getConeAngle());
    setParamValue("conePenumbra", light.getConePenumbra());
    setParamValue("attenuation", light.getAttenuation());
    setParamValue("cutoff", light.getCutoff());
    bind();
}
