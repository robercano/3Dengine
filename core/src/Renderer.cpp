/**
 * @class	Renderer
 * @brief	Interface for software/hardware renderers
 *
 * @author	Roberto Sosa Cano
 */

#include "Renderer.hpp"
#include "OpenGLRenderer.hpp"

Renderer * Renderer::_renderer = NULL;

Renderer *Renderer::GetRenderer(void)
{
	if(_renderer == NULL) {
		_renderer = new OpenGLRenderer();
	}
	return _renderer;
}

