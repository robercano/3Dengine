/**
 * @class	BlinnPhongShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "BlinnPhongShader.hpp"
#include "OpenGLBlinnPhongShader.hpp"

BlinnPhongShader *BlinnPhongShader::New(void)
{
    return new OpenGLBlinnPhongShader();
}

void BlinnPhongShader::Delete(BlinnPhongShader *target)
{
    delete target;
}

