/**
 * @class	OpenGLShaderMaterial
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string.h>
#include "OpenGLShaderMaterial.hpp"
#include <glm/glm.hpp>

OpenGLShaderMaterial::OpenGLShaderMaterial()
{
    setBlockName("Material");
    addParamName("ambient");
    addParamName("diffuse");
    addParamName("specular");
    addParamName("alpha");
    addParamName("shininess");
}

bool OpenGLShaderMaterial::copyMaterial(Material &material)
{
    setParamValue("ambient",   material.getAmbient());
    setParamValue("diffuse",   material.getDiffuse());
    setParamValue("specular",  material.getSpecular());
    setParamValue("alpha",     material.getAlpha());
    setParamValue("shininess", material.getShininess());
    bind();
    return true;
}
