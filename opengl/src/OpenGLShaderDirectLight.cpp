/**
 * @class	OpenGLShaderDirectLight
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "OpenGLShaderDirectLight.hpp"
#include <string.h>
#include <glm/glm.hpp>

void OpenGLShaderDirectLight::init(uint32_t bindingPoint)
{
    setBlockName("DirectLight");
    setBindingPoint(bindingPoint);
    addParamName("direction");
    addParamName("ambient");
    addParamName("diffuse");
    addParamName("specular");
}

void OpenGLShaderDirectLight::copyLight(DirectLight &light)
{
    setParamValue("direction", light.getDirection());
    setParamValue("ambient", light.getAmbient());
    setParamValue("diffuse", light.getDiffuse());
    setParamValue("specular", light.getSpecular());
    bind();
}
