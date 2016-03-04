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

#include "FXAARenderTarget.hpp"
#include "OpenGLFXAARenderTarget.hpp"

FXAARenderTarget *FXAARenderTarget::NewFXAARenderTarget(void)
{
    return new OpenGLFXAARenderTarget();
}

void FXAARenderTarget::DeleteFXAARenderTarget(FXAARenderTarget *target)
{
    delete target;
}

