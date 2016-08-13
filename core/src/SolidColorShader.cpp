/**
 * @class	SolidColorShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "SolidColorShader.hpp"
#include "OpenGLSolidColorShader.hpp"

SolidColorShader *SolidColorShader::New(void) { return new OpenGLSolidColorShader(); }
void SolidColorShader::Delete(SolidColorShader *target) { delete target; }
