/**
 * @class	NormalShadowMapShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "NormalShadowMapShader.hpp"
#include "OpenGLNormalShadowMapShader.hpp"

NormalShadowMapShader *NormalShadowMapShader::New(void)
{
    return new OpenGLNormalShadowMapShader();
}

void NormalShadowMapShader::Delete(NormalShadowMapShader *target)
{
    delete target;
}

