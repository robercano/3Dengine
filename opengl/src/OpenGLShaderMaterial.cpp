/**
 * @class	OpenGLShaderMaterial
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "OpenGLShaderMaterial.hpp"
#include <string.h>
#include <glm/glm.hpp>

void OpenGLShaderMaterial::init(uint32_t bindingPoint)
{
    setBlockName("Material");
    setBindingPoint(bindingPoint);
    addParamName("ambient");
    addParamName("diffuse");
    addParamName("specular");
    addParamName("alpha");
    addParamName("shininess");
}

void OpenGLShaderMaterial::copyMaterial(Material &material)
{
    setParamValue("ambient", material.getAmbient());
    setParamValue("diffuse", material.getDiffuse());
    setParamValue("specular", material.getSpecular());
    setParamValue("alpha", material.getAlpha());
    setParamValue("shininess", material.getShininess());
    bind();
}
