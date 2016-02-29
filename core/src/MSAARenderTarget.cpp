/**
 * @class	MSAARenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The MSAA render target applies no anti-aliasing
 *
 * @author	Roberto Sosa Cano (http://www.robertocano.es)
 */

#include "MSAARenderTarget.hpp"
#include "OpenGLMSAARenderTarget.hpp"

MSAARenderTarget *MSAARenderTarget::NewMSAARenderTarget(void)
{
    new OpenGLMSAARenderTarget();
}

void MSAARenderTarget::DeleteMSAARenderTarget(MSAARenderTarget *target)
{
    delete target;
}

