/**
 * @class	OpenGLShaderLight
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Light.hpp"
#include "OpenGL.h"
#include "OpenGLUniformBlock.hpp"

class OpenGLShaderLight : public OpenGLUniformBlock
{
  public:
    void init(uint32_t bindingPoint, uint32_t lightIndex);
    void copyLight(Light &light);
};
