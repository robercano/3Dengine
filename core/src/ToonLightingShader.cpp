/**
 * @class	ToonLightingShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "ToonLightingShader.hpp"
#include "OpenGLToonLightingShader.hpp"

ToonLightingShader *ToonLightingShader::New(void)
{
    return new OpenGLToonLightingShader();
}

void ToonLightingShader::Delete(ToonLightingShader *target)
{
    delete target;
}

