/**
 * @class	FlatShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "FlatShader.hpp"
#include "OpenGLFlatShader.hpp"

FlatShader *FlatShader::New(void) { return new OpenGLFlatShader(); }
void FlatShader::Delete(FlatShader *target) { delete target; }
