/**
 * @class	FXAARenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The FXAA render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "FXAA2RenderTarget.hpp"
#include "OpenGLFXAA2RenderTarget.hpp"

FXAA2RenderTarget *FXAA2RenderTarget::NewFXAA2RenderTarget(void)
{
    return new OpenGLFXAA2RenderTarget();
}

void FXAA2RenderTarget::DeleteFXAA2RenderTarget(FXAA2RenderTarget *target)
{
    delete target;
}

