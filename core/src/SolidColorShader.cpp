/**
 * @class	SolidColoringShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "SolidColoringShader.hpp"
#include "OpenGLSolidColoringShader.hpp"

SolidColoringShader *SolidColoringShader::New(void) { return new OpenGLSolidColoringShader(); }
void SolidColoringShader::Delete(SolidColoringShader *target) { delete target; }
