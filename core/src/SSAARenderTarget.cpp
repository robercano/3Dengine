/**
 * @class	SSAARenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The SSAA render target applies no anti-aliasing
 *
 * @author	Roberto Sosa Cano (http://www.robertocano.es)
 */

#include "SSAARenderTarget.hpp"
#include "OpenGLSSAARenderTarget.hpp"

SSAARenderTarget *SSAARenderTarget::NewSSAARenderTarget(void)
{
    new OpenGLSSAARenderTarget();
}

void SSAARenderTarget::DeleteSSAARenderTarget(SSAARenderTarget *target)
{
    delete target;
}

