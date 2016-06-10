/**
 * @class	Renderer
 * @brief	Interface for software/hardware renderers
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "Renderer.hpp"
#include "OpenGLRenderer.hpp"

Renderer *Renderer::_renderer = NULL;

Renderer *Renderer::GetInstance(void)
{
    if (_renderer == NULL) {
        _renderer = new OpenGLRenderer();
    }
    return _renderer;
}

void Renderer::DisposeInstance(void)
{
    delete _renderer;
    _renderer = NULL;
}
