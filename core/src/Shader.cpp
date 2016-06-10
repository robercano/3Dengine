/**
 * @class	Renderer
 * @brief	Interface for software/hardware renderers
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "Shader.hpp"
#include "OpenGLShader.hpp"

Shader *Shader::New(void) { return new OpenGLShader(); }
void Shader::Delete(Shader *shader) { delete shader; }
