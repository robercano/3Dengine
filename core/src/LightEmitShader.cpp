/**
 * @class	LightEmitShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "LightEmitShader.hpp"
#include "OpenGLLightEmitShader.hpp"

LightEmitShader *LightEmitShader::New(void) { return new OpenGLLightEmitShader(); }
void LightEmitShader::Delete(LightEmitShader *target) { delete target; }
